use rand::Rng;
use std::cmp::Ordering;

slint::include_modules!();

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    // secret number
    let secret_number = rand::thread_rng().gen_range(1..=100);

    ui.on_handle_guess_event({
        let ui_handle = ui.as_weak();
        move || {
            let ui = ui_handle.unwrap();
            let guess = ui.get_guess();

            let guess: Option<u32> = guess.trim().parse().ok();

            if let Some(num) = guess {
                let response = match num.cmp(&secret_number) {
                    Ordering::Less => "Too small",
                    Ordering::Equal => "Correct",
                    Ordering::Greater => "Too big",
                };
                ui.set_response(response.into());
            } else {
                ui.set_response("Invalid guess".into());
            }
        }
    });

    ui.run()
}
