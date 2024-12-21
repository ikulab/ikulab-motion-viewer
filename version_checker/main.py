import os
import subprocess
import sys
import webbrowser
import packaging.version

from ui import MainUi
from version_check import get_latest_release_version

version_info_dir = ""


def launch_app():
    app_path = sys.argv[2]
    subprocess.Popen([app_path])


def on_close():
    launch_app()


def on_later():
    launch_app()


def on_skip():
    global version_info_dir

    get_succeeded, latest_version = get_latest_release_version()

    if get_succeeded:
        with open(f"{version_info_dir}/skip_version.txt", "w") as f:
            f.write(latest_version)

    launch_app()


def on_open():
    webbrowser.open("https://github.com/ikulab/ikulab-motion-viewer/releases/latest")


def main():
    global version_info_dir

    get_succeeded, latest_version = get_latest_release_version()

    if not get_succeeded:
        launch_app()
        return

    version_info_dir = sys.argv[1]

    with open(f"{version_info_dir}/current_version.txt", "r") as f:
        current_version = f.read()

    with open(f"{version_info_dir}/skip_version.txt", "r") as f:
        skip_version = f.read()

    open_update_dialog = False

    # if new version is available
    if packaging.version.parse(current_version) < packaging.version.parse(latest_version):
        # set true if latest version is not skipped
        open_update_dialog = skip_version != latest_version

    if open_update_dialog:
        app = MainUi(latest_version, on_open, on_close, on_later, on_skip)
        app.MainLoop()
    else:
        launch_app()


if __name__ == "__main__":
    main()
