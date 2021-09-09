import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Common

TransparentPane {
   id: root
   padding: 5

   RowLayout {

      Image {
         source: "qrc:/Images/HostSecureLogo.png"
         sourceSize.height: 50
         sourceSize.width: 50
      }

      Label {
         text: "Host Secure"
         font.pixelSize: 25
         font.bold: true
      }
   }

}
