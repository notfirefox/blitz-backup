use directories::ProjectDirs;
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

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    if let Ok(settings) = load_settings() {
        ui.set_backup_repository(settings.repository.into());
        ui.set_backup_password(settings.password.into());
    }

    ui.on_save_settings({
        let ui_handle = ui.as_weak();
        move || {
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

    ui.run()
}
