import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Common

Pane {
   padding: 30
   background: null

   ListView {
      id: edgeListView
      model: HSC.edgeModel
      anchors.fill: parent
      anchors.topMargin: 10
      anchors.leftMargin: 10
      ScrollBar.vertical: ScrollBar { }
      delegate: TransparentPane {
         width: ListView.view.width
         radius: 20

         contentItem: Item {

            ColumnLayout {
               RowLayout {
                  Label {
                     text: model.id
                  }
                  Label {
                     text: model.isOnline
                  }
               }

               RowLayout {
                  Repeater {
                     model: deviceModel
                     delegate: ColumnLayout {
                        Label { text: model.lastHeartBeat }
                        Label { text: model.deviceId }
                        Label { text: model.deviceSerial }
                        Label { text: model.target }
                        Label { text: model.event }
                        Label { text: model.interface }
                     }
                  }
               }
            }
         }
      }
   }

   TransparentPane {
      visible: edgeListView.count === 0
      anchors.centerIn: parent
      radius: 5

      Label {
         anchors.centerIn: parent
         text: "No edges connected.."
         enabled: false
         font.bold: true
         font.pixelSize: 25
      }
   }
}
