import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Common

TransparentPane {

   ListView {
      model: HSC.usbDeviceModel
      anchors.fill: parent
      anchors.margins: 20
      ScrollBar.vertical: ScrollBar { }
      delegate: ItemDelegate {
         width: ListView.view.width

         contentItem: RowLayout {
            Label {
               text: model.name
            }
            Label {
               text: model.approved
            }
         }
      }
   }
}
