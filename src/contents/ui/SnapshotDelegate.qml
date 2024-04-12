import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import io.github.notfirefox 1.0

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
                        text: Qt.formatDateTime(model.time, "dd.MM.yyyy – HH:mm:ss")
                        level: 1
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft
                    }
                    Controls.Label {
                        text: model.shortId
                        Layout.alignment: Qt.AlignRight
                    }
                }
                Kirigami.Separator {
                    Layout.fillWidth: true
                }
                Controls.Label {
                    text: "Host: " + model.hostname
                }
                Controls.Label {
                    text: "Paths: " + model.paths
                }
                Controls.Label {
                    text: "Tags: " + model.tags
                }
                RowLayout {
                    Layout.alignment: Qt.AlignRight

                    Controls.Button {
                        text: i18n("Mount")
                        onClicked: Backend.mount(model.shortId)
                    }
                    Controls.Button {
                        text: i18n("Restore")
                        onClicked: Backend.restore(model.shortId)
                    }
                }
            }
        }
    }
}
