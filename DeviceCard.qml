import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: card
    
    property string deviceName: ""
    property string deviceType: ""
    property real setPoint: 0
    property string state
    
    signal clicked()

    width: parent.width
    height: 80
    radius: 12
    color: "#222222"
    opacity: 0.9
    
    // Subtle border
    // border.color: "#333333"
    // border.width: 1

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        spacing: 12

        // Icon
        Rectangle {
            width: 48
            height: 48
            radius: 8
            color: "#2a2a2a"

            Text {
                anchors.centerIn: parent
                text: "❄️"
                font.pixelSize: 24
            }
        }

        ColumnLayout {
            spacing: 2

            Text {
                text: card.deviceName.charAt(0).toUpperCase() + card.deviceName.slice(1)
                font.pixelSize: 16
                font.weight: Font.Medium
                color: "#ffffff"
            }

            Text {
                text: card.state.charAt(0).toUpperCase() + card.state.slice(1)
                font.pixelSize: 12
                color: "#888888"
            }
        }

        Rectangle {
            id: separator
            Layout.fillWidth: true
            color: "transparent"
            
        }

        // Set Point Display
        Text {
            text: card.setPoint.toFixed(1) + "°C"
            font.pixelSize: 18
            font.weight: Font.Bold
            color: "#BDBDBD"
        }
    }

    // Touch feedback
    // Rectangle {
    //     anchors.fill: parent
    //     radius: parent.radius
    //     color: "#ffffff"
    //     opacity: 0
        
    //     Behavior on opacity {
    //         NumberAnimation { duration: 150 }
    //     }
        
    //     id: touchFeedback
    // }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onPressed: {
            card.color = "#171717"
        }

        onReleased: {
            card.color = "#222222"
        }

        // onCanceled: {
        //     filler.opacity = 0.03
        // }
        
        onClicked: {
            card.clicked()
        }

        onEntered: {
            card.color = "#222222"
        }
        onExited: {
            card.color = "#222222"
            // filler.opacity = 0.03
        }
    }
}
