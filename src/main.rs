use directories::ProjectDirs;
use rustic_backend::BackendOptions;
use rustic_core::{repofile::SnapshotFile, Repository, RepositoryOptions, RusticError};
use serde::{Deserialize, Serialize};
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

fn get_snapshots(repo_str: &str, pwd_str: &str) -> Result<Vec<SnapshotFile>, RusticError> {
    let repo_options = RepositoryOptions::default().password(pwd_str);

    let backends = BackendOptions::default()
        .repository(repo_str)
        .to_backends()
        .expect("Failed to create rustic backends");

    let repo = Repository::new(&repo_options, backends)?.open()?;
    repo.get_all_snapshots()
}

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    let settings = load_settings().map_or_else(
        |_| Settings {
            repository: String::new(),
            password: String::new(),
        },
        |settings| settings,
    );
    ui.set_backup_repository(settings.repository.clone().into());
    ui.set_backup_password(settings.password.clone().into());

    ui.on_save_settings({
        let ui_handle = ui.as_weak();
        move || {
            // TODO: Don't use unwrap
            let ui = ui_handle.unwrap();
            let settings = Settings {
                repository: ui.get_backup_repository().to_string(),
                password: ui.get_backup_password().to_string(),
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

    ui.on_refresh_snapshots({
        move || {
            // TODO: Show snapshots in the gui instead of the cli
            if let Ok(snapshots) = get_snapshots(&settings.repository, &settings.password) {
                println!("ID | Time | Host | Tags | Path");
                for s in snapshots {
                    println!(
                        "{} | {} | {} | {} | {}",
                        s.id, s.time, s.hostname, s.tags, s.paths
                    );
                }
            } else {
                println!("Failed to load snapshots from repo");
            }
        }
    });

    ui.run()
}
