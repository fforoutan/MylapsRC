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

    color: "#2b2b2b"
    QtObject {
        id: theme
        property color backgroundColor: "#2b2b2b"
        property color textColor: "#ffffff"
        property color buttonColor: "#3b3b3b"
        property color buttonTextColor: "#ffffff"
        property color borderColor: "#757575"
    }

    Rectangle {
        anchors.fill: parent
        color: theme.backgroundColor
        Image {
            id: imageID
            source: "qrc:/logo-mylaps.svg"
            width: 200
            height: 200
            fillMode: Image.PreserveAspectFit
        }

        RowLayout {
            id: rowll
            width: parent.width
            Layout.alignment: Qt.AlignVCenter
            anchors.top: imageID.bottom
            height: 50
            spacing: 0

            Button {
                text: qsTr("Upload Race Data")
                onClicked: client.sendCSVData()
                width: 200
                height: 50
                background: Rectangle {
                    color: theme.buttonColor
                    radius: 10
                    border.color: theme.borderColor
                }
                contentItem: Text {
                    text: qsTr("Upload Race Data")
                    color: theme.buttonTextColor
                    font.pixelSize: 16
                }
            }


            Label {
                text: qsTr("Driver Number: ")
                color: theme.textColor
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
                color: theme.buttonTextColor
                background: Rectangle {
                    color: theme.buttonColor
                    radius: 10
                    border.color: theme.borderColor
                }
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
                    } else {
                        client.requestAverageTime(driverNumberInput.text)
                    }
                }
                width: 150
                height: 40
                background: Rectangle {
                    color: theme.buttonColor
                    radius: 10
                    border.color: theme.borderColor
                }
                contentItem: Text {
                    id: contentTextID
                    text: qsTr("Get Avg Time")
                    color: theme.buttonTextColor
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }
            }
        }
        Rectangle {
            width: parent.width
            color: theme.buttonColor
            radius: 10
            border.color: theme.borderColor
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
                color: theme.textColor
                font.pixelSize: 16
                background: Rectangle {
                    color: theme.buttonColor
                    radius: 10
                    border.color: theme.borderColor
                }
                padding: 10
            }
        }
        Connections {
            target: client
            function onResponseReceived(response) {
                responseArea.text = response;
            }
        }
    }
}
