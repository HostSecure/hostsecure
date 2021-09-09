import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Common

TransparentPane {
   padding: 30

   ListView {
      model: HSC.edgeModel
      anchors.fill: parent
      anchors.margins: 50
      ScrollBar.vertical: ScrollBar { }
      delegate: ItemDelegate {
         width: ListView.view.width

         contentItem: TransparentPane {
            radius: 20

            RowLayout {
               Label {
                  text: model.name
               }
               Label {
                  text: model.status
               }
            }
         }
      }
   }
}
