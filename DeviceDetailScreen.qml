import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: detailPage
    
    property var device: null
    
    signal backRequested()
    
    background: Rectangle {
        // color: "#0a0a0a"
        color: "#121212"
    }
    
    header: ToolBar {
        height: 70
        background: Rectangle {
            color: "#171717"
            
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#2a2a2a"
            }
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            // anchors.topMargin: 10
            // anchors.bottomMargin: 10
            spacing: 5
            
            Button {
                // icon.source: "qrc:/qt/qml/smartHome/back-icon.png"
                text: "←"
                font.pixelSize: 28
                font.bold: true
                flat: true
                
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: "#BDBDBD"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                background: Rectangle {
                    color: "transparent"
                }
                
                onClicked: detailPage.backRequested()
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                
                Label {
                    text: device ? device.name.charAt(0).toUpperCase() + device.name.slice(1) : ""
                    color: "#ffffff"
                    font.pixelSize: 24
                    font.weight: Font.Bold
                }
                
                Label {
                    text: device ? device.type.charAt(0).toUpperCase() + device.type.slice(1) : ""
                    color: "#BDBDBD"
                    font.pixelSize: 14
                }
            }
        }   
    }
    
    ScrollView {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 16
            
            // Sensors Section
            Label {
                text: "Sensors"
                color: "#888888"
                font.pixelSize: 12
                Layout.topMargin: 8
            }
            
            SensorCard {
                Layout.fillWidth: true
                sensorName: "Temperature Sensor"
                currentValue: detailPage.device ? detailPage.device.temperature : 0
                unit: "°C"
            }

            DeviceCard {
                Layout.fillWidth: true
                deviceName: "hola"
                deviceType: "mofaka"
                setPoint: 20
            }

            
            // Controls Section
            Label {
                text: "Controls"
                color: "#888888"
                font.pixelSize: 12
                font.weight: Font.Medium
                Layout.topMargin: 16
            }
            
            OutputCard {
                Layout.fillWidth: true
                outputName: "Cooling Control"
                setPoint: detailPage.device ? detailPage.device.setPoint : 20
                minValue: 0
                maxValue: 30
                
                // if this was tied to an output object via model, i could call directli model.data.setSetPoint
                onSetPointChanged: detailPage.device.setSetPoint(setPoint)
            }
            
            Item {

                Layout.fillWidth: true
                height: 20
            }
        }
    }
}
