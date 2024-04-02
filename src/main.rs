use chrono::{DateTime, Local};
use core::fmt;
use directories::ProjectDirs;
use rustic_backend::BackendOptions;
use rustic_core::{
    repofile::SnapshotFile, BackupOptions, ConfigOptions, KeyOptions, NoProgressBars, OpenStatus,
    PathList, Repository, RepositoryOptions, RusticError, RusticResult, SnapshotOptions,
    StringList,
};
use serde::{Deserialize, Serialize};
use slint::ComponentHandle;
use std::{
    fs::{self},
    path::PathBuf,
};

extern crate directories;

slint::include_modules!();

fn get_project_dirs() -> Result<ProjectDirs, std::io::Error> {
    let dirs = ProjectDirs::from("io.github", "notfirefox", "Blitz Backup");

    dirs.ok_or_else(|| {
        let kind = std::io::ErrorKind::NotFound;
        let message = "Failed to find project dirs";
        std::io::Error::new(kind, message)
    })
}

fn get_config_dir() -> Result<PathBuf, std::io::Error> {
    let dirs = get_project_dirs();
    Ok(PathBuf::from(dirs?.config_dir()))
}

#[derive(Serialize, Deserialize, Debug)]
struct Settings {
    repository: String,
    password: String,
    pathlist: String,
}

fn load_settings() -> Result<Settings, std::io::Error> {
    let config_file = get_config_dir()?.join("config.json");
    let data = fs::read_to_string(config_file)?;
    let result = serde_json::from_str(&data);
    if let Ok(settings) = result {
        return Ok(settings);
    }

    Err(std::io::Error::new(
        std::io::ErrorKind::NotFound,
        "Failed to deserialize settings data",
    ))
}

fn save_settings(settings: &Settings) -> Result<(), std::io::Error> {
    let config_dir = get_config_dir()?;
    let config_file = config_dir.join("config.json");
    fs::create_dir_all(config_dir)?;

    let serialized = serde_json::to_string(&settings);
    fs::write(config_file, serialized?)
}

struct Snapshot {
    id: rustic_core::Id,
    time: DateTime<Local>,
    hostname: String,
    tags: StringList,
    paths: StringList,
}

impl fmt::Display for Snapshot {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{} | {} | {} | {} | {}",
            self.id, self.time, self.hostname, self.tags, self.paths
        )
    }
}

impl From<SnapshotFile> for Snapshot {
    fn from(value: SnapshotFile) -> Self {
        Self {
            id: value.id,
            time: value.time,
            tags: value.tags,
            hostname: value.hostname,
            paths: value.paths,
        }
    }
}

fn open_repository(
    repo_str: &str,
    pwd_str: &str,
) -> RusticResult<Repository<NoProgressBars, OpenStatus>> {
    let repo_options = RepositoryOptions::default().password(pwd_str);

    let backends = BackendOptions::default()
        .repository(repo_str)
        .to_backends()
        .expect("Failed to create rustic backends");

    Repository::new(&repo_options, backends)?.open()
}

fn init_repository(
    repo_str: &str,
    pwd_str: &str,
) -> RusticResult<Repository<NoProgressBars, OpenStatus>> {
    let repo_options = RepositoryOptions::default().password(pwd_str);

    let backends = BackendOptions::default()
        .repository(repo_str)
        .to_backends()
        .expect("Failed to create rustic backends");

    let key_opts = KeyOptions::default();
    let config_ops = ConfigOptions::default();

    let repo = Repository::new(&repo_options, backends);
    repo?.init(&key_opts, &config_ops)
}

fn get_snapshots(repo_str: &str, pwd_str: &str) -> Result<Vec<SnapshotFile>, RusticError> {
    let repo = open_repository(repo_str, pwd_str);
    repo?.get_all_snapshots()
}

fn do_backup(repo_str: &str, pwd_str: &str, pathlist_str: &str) -> RusticResult<SnapshotFile> {
    let repo = open_repository(repo_str, pwd_str);
    let repo = repo?.to_indexed_ids()?;

    let snap = SnapshotOptions::default().to_snapshot()?;

    let backup_opts = BackupOptions::default();
    let source = PathList::from_string(pathlist_str)?.sanitize()?;

    repo.backup(&backup_opts, &source, snap)
}

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    let settings = load_settings().map_or_else(
        |_| Settings {
            repository: String::new(),
            password: String::new(),
            pathlist: String::new(),
        },
        |settings| settings,
    );
    ui.set_backup_repository(settings.repository.into());
    ui.set_backup_password(settings.password.into());
    ui.set_backup_pathlist(settings.pathlist.into());

    ui.on_save_settings({
        let ui_handle = ui.as_weak();
        move || {
            // TODO: Don't use unwrap
            let ui = ui_handle.unwrap();
            let settings = Settings {
                repository: ui.get_backup_repository().to_string(),
                password: ui.get_backup_password().to_string(),
                pathlist: ui.get_backup_pathlist().to_string(),
            };
            let result = save_settings(&settings);
            match result {
                Ok(()) => {}
                Err(error) => {
                    println!("Error: {error}");
                }
            }
        }
    });

    ui.on_init_repository({
        let ui_handle = ui.as_weak();
        move || {
            let ui = ui_handle.unwrap();
            let repo = ui.get_backup_repository().to_string();
            let pwd = ui.get_backup_password().to_string();
            let result = init_repository(&repo, &pwd);
            match result {
                Ok(_) => {
                    println!("Successfully initialized repository.");
                }
                Err(error) => {
                    eprintln!("Failed to initialize repository: {error}");
                }
            }
        }
    });

    ui.on_backup({
        let ui_handle = ui.as_weak();
        move || {
            let ui = ui_handle.unwrap();
            let repo = ui.get_backup_repository().to_string();
            let pwd = ui.get_backup_password().to_string();
            let pathlist = ui.get_backup_pathlist();
            let result = do_backup(&repo, &pwd, &pathlist);
            match result {
                Ok(snapshot) => {
                    let snapshot = Snapshot::from(snapshot);
                    println!("Successfully created snapshot: {snapshot}");
                }
                Err(error) => {
                    eprint!("Failed to create snapshot: {error}");
                }
            }
            // TODO: Backup repo
        }
    });

    ui.on_refresh_snapshots({
        let ui_handle = ui.as_weak();
        move || {
            // TODO: Don't use unwrap
            let ui = ui_handle.unwrap();
            let repo = ui.get_backup_repository().to_string();
            let pwd = ui.get_backup_password().to_string();

            // TODO: Show snapshots in the gui instead of the cli
            let result = get_snapshots(&repo, &pwd);
            match result {
                Ok(snapshots) => {
                    let snapshots: Vec<Snapshot> =
                        snapshots.into_iter().map(Snapshot::from).collect();
                    println!("ID | Time | Host | Tags | Path");
                    for s in snapshots {
                        println!("{s}");
                    }
                }
                Err(error) => {
                    eprintln!("Failed loading snapshots: {error}");
                }
            }
        }
    });

    // TODO: Implement mount command
    ui.on_mount({
        // let ui_handle = ui.as_weak();
        move || {
            // let ui = _ui_handle.unwrap();
            println!("TODO: Implement mount command.");
        }
    });

    // TODO: Implement restore command
    ui.on_restore({
        // let ui_handle = ui.as_weak();
        move || {
            // let ui = _ui_handle.unwrap();
            println!("TODO: Implement restore command.");
        }
    });

    // TODO: Implement forget command
    ui.on_forget({
        // let ui_handle = ui.as_weak();
        move || {
            // let ui = _ui_handle.unwrap();
            println!("TODO: Implement forget command.");
        }
    });

    ui.run()
}
