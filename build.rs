fn main() {
    let compile_result = slint_build::compile("ui/appwindow.slint");
    compile_result.expect("Compilation of appwindow ui file failed.");
}
