import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: outputCard
    
    property string outputName: ""
    property real setPoint: 20 // this should expose the slider value
    property real minValue: 0
    property real maxValue: 30
    
    height: 120
    radius: 12
    
    opacity: 0.9
    color: "#222222"
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            Rectangle {
                width: 48
                height: 48
                radius: 8
                color: "#2a2a2a"
                
                Text {
                    anchors.centerIn: parent
                    text: "⚙️"
                    font.pixelSize: 24
                }
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                
                Text {
                    text: outputCard.outputName
                    font.pixelSize: 17
                    font.weight: Font.Medium
                    color: "#ffffff"
                }
                
                Text {
                    text: "Set target temperature"
                    font.pixelSize: 13
                    color: "#888888"
                }
            }

            Rectangle {
                id: separator
                color: "transparent"
                Layout.fillWidth: true
            }
                
            Text {
                text: slider.value.toFixed(1) + "°C"
                font.pixelSize: 18
                font.weight: Font.Bold
                color: "#BDBDBD"
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            Text {
                text: outputCard.minValue.toFixed(0) + "°C"
                font.pixelSize: 14
                color: "#666666"
            }
            
            Slider {
                id: slider
                Layout.fillWidth: true
                from: outputCard.minValue
                to: outputCard.maxValue
                stepSize: 0.1
                live: true // only update value when released

                // value: 20
                value: outputCard.setPoint

                onPressedChanged: outputCard.setPoint = value
                
                background: Rectangle {
                    x: slider.leftPadding
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    width: slider.availableWidth
                    height: 6
                    radius: 3
                    color: "#3a3a3a"
                    
                    Rectangle {
                        width: slider.visualPosition * parent.width
                        height: parent.height
                        opacity: 0.8
                        color: "#BDBDBD"
                        radius: 3
                    }
                }
                
                handle: Rectangle {
                    x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    implicitWidth: 18
                    implicitHeight: 18
                    radius: height / 2
                    // color: slider.pressed ? "#CDCDCD" : "#ffffff"
                    color: "#ffffff"
                    opacity: slider.pressed ? 0.8 : 1
                }
            }
            
            Text {
                text: outputCard.maxValue.toFixed(0) + "°C"
                font.pixelSize: 14
                color: "#666666"
            }
        }
    }
}
