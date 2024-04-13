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

            GridLayout {
                Kirigami.Icon {
                    source: "application-x-compress"
                }
                ColumnLayout {
                    Kirigami.Heading {
                        text: Qt.formatDateTime(model.time, "dd.MM.yyyy – HH:mm")
                        level: 1
                    }
                    Controls.Label {
                        text: model.hostname
                    }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    Rectangle {
                        Layout.fillWidth: true
                    }
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
