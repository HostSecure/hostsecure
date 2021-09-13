import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Common

Pane {
   background: null

   TransparentPane {
      radius: 20
      padding: 20

      ColumnLayout {

         Label {
            text: "General"
            font.pixelSize: 24
            font.underline: true
         }

         Repeater {
            model: [
               { title: "Connected edges", value: "2" },
               { title: "Connected devices", value: "4" },
               { title: "Something cool", value: "YAY!" },
            ]
            delegate: RowLayout {
               Label {
                  text: modelData.title
                  font.pixelSize: 16
                  Layout.preferredWidth: 140
               }
               Label {
                  text: modelData.value
                  font.pixelSize: 16
                  font.bold: true
               }
            }
         }

      }
   }
}
