import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: sensorCard
    
    property string sensorName: ""
    property real currentValue: 0
    property string unit: ""
    
    width: parent.width
    height: 80
    radius: 12
    
    color: "#222222"
    opacity: 0.9
    
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        spacing: 12
        
        Rectangle {
            width: 48
            height: 48
            radius: 8
            color: "#2a2a2a"

            Text {
                anchors.centerIn: parent
                text: "🌡️"
                font.pixelSize: 24
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            
            Text {
                text: sensorCard.sensorName
                font.pixelSize: 17
                font.weight: Font.Medium
                color: "#ffffff"
            }
            
            Text {
                text: "Current reading"
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
            text: sensorCard.currentValue.toFixed(1) + sensorCard.unit
            font.pixelSize: 18
            font.weight: Font.Bold
            color: "#BDBDBD"
        }
    }
}
