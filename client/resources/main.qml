import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    minimumWidth: 600
    minimumHeight:550
    title: qsTr("Client Application")

    Rectangle {
        anchors.fill: parent

        RowLayout {
            id: rowll
            width: parent.width
            Layout.alignment: Qt.AlignVCenter
            height: 50
            spacing: 0

            Button {
                text: qsTr("Upload Race Data")
                onClicked: client.sendCSVData()
                width: 200
                height: 50
                background: Rectangle {
                    radius: 10
                }
                contentItem: Text {
                    text: qsTr("Upload Race Data")
                    font.pixelSize: 16
                }
            }


            Label {
                text: qsTr("Driver Number: ")
                id: driverLabel
                Layout.alignment: Qt.AlignRight
                font.pixelSize: 16
                padding: 10
            }

            TextField {
                id: driverNumberInput
                placeholderText: qsTr("Enter Driver Number")
                width: 150
                height: 40
                Layout.alignment: Qt.AlignLeft
                font.pixelSize: 16
                padding: 10
            }
            Button {
                text: qsTr("Get Avg Time")
                id: avgButton
                Layout.alignment: Qt.AlignJustify
                onClicked: {
                    if (!driverNumberInput.text.match(/^\d+$/)) {
                        driverNumberInput.clear()
                        driverNumberInput.placeholderText = qsTr("Insert positive Numbers")
                    }
                }
                width: 150
                height: 40
                background: Rectangle {
                    radius: 10
                }
                contentItem: Text {
                    id: contentTextID
                    text: qsTr("Get Avg Time")
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }
            }
        }
        Rectangle {
            width: parent.width
            radius: 10
            anchors.top: rowll.bottom
            anchors.bottom:parent.bottom
            height: 300

            TextArea {
                id: responseArea
                width: parent.width
                height: 300
                clip: true
                readOnly: true
                text: "results:"
                font.pixelSize: 16
                background: Rectangle {
                    radius: 10
                }
                padding: 10
            }
        }
    }
}
