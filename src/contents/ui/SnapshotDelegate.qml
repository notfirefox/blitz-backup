import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami

Kirigami.AbstractCard {
    id: snapshotDelegate
    contentItem: Item {
        implicitWidth: delegateLayout.implicitWidth
        implicitHeight: delegateLayout.implicitHeight
        GridLayout {
            id: delegateLayout
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            rowSpacing: Kirigami.Units.largeSpacing
            columnSpacing: Kirigami.Units.largeSpacing
            columns: root.wideScreen ? 4 : 2

            ColumnLayout {
                RowLayout {
                    Layout.fillWidth: true
                    Kirigami.Heading {
                        text: Qt.formatDateTime(snapshotTime, "dd.MM.yyyy – HH:mm:ss")
                        level: 1
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft
                    }
                    Controls.Label {
                        text: snapshotId
                        Layout.alignment: Qt.AlignRight
                    }
                }
                Kirigami.Separator {
                    Layout.fillWidth: true
                }
                Controls.Label {
                    text: "Host: " + snapshotHost
                }
                Controls.Label {
                    text: "Paths: " + snapshotPaths
                }
                Controls.Label {
                    text: "Tags: " + snapshotTags
                }
                RowLayout {
                    Layout.alignment: Qt.AlignRight

                    Controls.Button {
                        text: i18n("Mount")
                    }
                    Controls.Button {
                        text: i18n("Restore")
                    }
                }
            }
        }
    }
}
