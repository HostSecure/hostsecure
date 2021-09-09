import QtQuick
import QtQuick.Controls
import Common

Pane {
   id: root

   property int radius: 1

   Component.onCompleted: {
      background.color = Colors.paneBackgroundColor
      background.border.color = Colors.paneBorderColor
      background.radius = Qt.binding( function() { return root.radius } )
   }
}
