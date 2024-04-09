import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import io.github.notfirefox 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Blitz Backup")

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: true
        actions: [
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "gtk-quit"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        ]
    }

    ListModel {
        id: snapshotModel
        ListElement {
            snapshotId: "85da9c7f"
            snapshotTime: "2024-04-09 10:17:16"
            snapshotHost: "desktop"
            snapshotPaths: "/home/notfirefox"
            snapshotTags: "some tags"
        }
        ListElement {
            snapshotId: "e08ebe6e"
            snapshotTime: "2024-04-08 11:35:45"
            snapshotHost: "laptop"
            snapshotPaths: "/home/notfirefox"
            snapshotTags: "some tags"
        }
        ListElement {
            snapshotId: "a83f76be"
            snapshotTime: "2024-04-08 10:23:08"
            snapshotHost: "desktop"
            snapshotPaths: "/home/notfirefox"
            snapshotTags: "some tags"
        }
        ListElement {
            snapshotId: "8632cc8e"
            snapshotTime: "2024-04-07 13:04:26"
            snapshotHost: "desktop"
            snapshotPaths: "/home/notfirefox"
            snapshotTags: "some tags"
        }
        ListElement {
            snapshotId: "9f9ad7d9"
            snapshotTime: "2024-04-06 10:47:54"
            snapshotHost: "desktop"
            snapshotPaths: "/home/notfirefox"
            snapshotTags: "some tags"
        }
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        title: i18nc("@title", "Snapshots")

        // Kirigami.Action encapsulates a UI action. Inherits from Controls.Action
        actions {
            main: Kirigami.Action {
                id: addAction
                // Name of icon associated with the action
                icon.name: "list-add"
                // Action text, i18n function returns translated string
                text: i18nc("@action:button", "Backup")
                onTriggered: Backend.backup()
            }
            contextualActions: [
                Kirigami.Action {
                    icon.name: "view-refresh"
                    text: i18nc("@action:button", "Refresh")
                    onTriggered: Backend.refresh()
                }
            ]
        }

        Kirigami.CardsListView {
            id: layout
            model: snapshotModel
            delegate: SnapshotDelegate {
            }
        }
    }
}
