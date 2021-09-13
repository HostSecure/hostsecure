import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Common

Pane {
   id: root
   Material.background: Colors.paneBackgroundColor

   property int radius: 1

   Component.onCompleted: {
//      background.color =
      background.border.color = Colors.paneBorderColor
      background.radius = Qt.binding( function() { return root.radius } )
   }
}
