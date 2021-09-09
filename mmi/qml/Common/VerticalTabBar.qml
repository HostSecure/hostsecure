import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ListView {
   implicitHeight: contentItem.childrenRect.height
   implicitWidth: contentItem.childrenRect.width

   orientation: ListView.Vertical
   boundsBehavior: Flickable.StopAtBounds
   flickableDirection: Flickable.AutoFlickIfNeeded
   snapMode: ListView.SnapToItem

   highlightMoveDuration: 250
   highlightResizeDuration: 0
   highlightFollowsCurrentItem: true
   highlightRangeMode: ListView.ApplyRange
   preferredHighlightBegin: 48
   preferredHighlightEnd: width - 48

   highlight: Item {
      z: 2
      Rectangle {
         width: 5
         height: parent.height
         color: Material.accentColor
      }
   }
}
