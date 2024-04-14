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
            },
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "settings-configure"
                shortcut: StandardKey.Preferences
                onTriggered: pageStack.layers.push(settingsPage)
            },
            Kirigami.Action {
                text: i18n("About")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push(aboutPage)
            }
        ]
    }

    Component {
        id: settingsPage

        Kirigami.Page {
            Kirigami.FormLayout {
                anchors.fill: parent
                Item {
                    Kirigami.FormData.isSection: true
                    Kirigami.FormData.label: "General"
                }

                Controls.TextField {
                    Kirigami.FormData.label: "Repository"
                    text: Config.repository
                    onTextChanged: {
                        Config.repository = text;
                    }
                }
                Controls.TextField {
                    Kirigami.FormData.label: "password"
                    text: Config.password
                    onTextChanged: {
                        Config.password = text;
                    }
                }
                Controls.Button {
                    id: applyButton
                    text: i18n("Apply")
                    onClicked: Config.save()
                }
            }
        }
    }

    Component {
        id: aboutPage

        Kirigami.AboutPage {
            aboutData: About
        }
    }

    SnapshotModel {
        id: snapshotModel
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        title: i18nc("@title", snapshotList.count + " Entries")

        // Kirigami.Action encapsulates a UI action. Inherits from Controls.Action
        actions: [
            Kirigami.Action {
                id: addAction
                icon.name: "list-add"
                text: i18nc("@action:button", "Backup")
                onTriggered: Backend.backup()
            },
            Kirigami.Action {
                id: refreshAction
                icon.name: "view-refresh"
                text: i18nc("@action:button", "Refresh")
                onTriggered: Backend.refresh()
            }
        ]

        Kirigami.CardsListView {
            id: snapshotList
            model: snapshotModel
            delegate: SnapshotDelegate {
            }
            Kirigami.PlaceholderMessage {
                anchors.centerIn: parent
                width: parent.width - Kirigami.Units.largeSpacing * 4
                visible: snapshotList.count === 0
                text: "No entries"
                explanation: "Click Backup to create a new backup"
            }
        }

        Component.onCompleted: {
            Backend.snapshotModelUpdated.connect(snapshotModel.updateSnapshotModel);
        }
    }
}
