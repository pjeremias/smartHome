import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 800
    //title: "Smart Home"

    background: Rectangle {
        // color: "#0a0a0a"
        color: "#121212"
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainView
    }
    
    Component {
        id: mainView
        
        Page {
            background: Rectangle {
                color: "transparent"
            }
            
            header: ToolBar {
                height: 70
                background: Rectangle {
                    // color: "#0a0a0a"
                    color: "#171717"
                    
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: "#2a2a2a"
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    anchors.topMargin: 10
                    anchors.bottomMargin: 10
                    spacing: 4

                    Label {
                        text: "Smart Home"
                        color: "#ffffff"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                    }

                    Label {
                        text: "Dashboard"
                        color: "#BDBDBD"
                        font.pixelSize: 12
                    }
                }
            }

            ScrollView {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.topMargin: 12
                anchors.bottomMargin: 12

                ColumnLayout {
                    width: parent.width
                    spacing: 12

                    Repeater {
                        model: deviceManager.devices

                        DeviceCard {
                            Layout.fillWidth: true
                            deviceName: modelData.name
                            deviceType: modelData.type
                            setPoint: modelData.setPoint
                            state: modelData.state
                            
                            onClicked: {
                                stackView.push(detailView, { device: modelData })
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        height: 20
                    }
                }
            }
        }
    }

    Component {
        id: detailView
        DeviceDetailScreen {
            onBackRequested: {
                stackView.pop()
            }
        }
    }

}
