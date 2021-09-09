import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Common

Item {

   RowLayout {
      anchors.fill: parent
      spacing: 0

      TransparentPane {
         padding: 0
         Material.elevation: 3
         Material.background: Colors.setupBackgroundColor
         Layout.fillHeight: true
         z: 10

         VerticalTabBar {
            id: setupBar
            currentIndex: 0
            model: [
               { title: "Home",    iconName: "home" },
               { title: "Edges",   iconName: "computer" },
               { title: "Devices", iconName: "memory-stick" },
            ]
            delegate: TabButton {
               width: 100
               icon.height: 30
               icon.width: 30
               icon.name: modelData.iconName
               text: modelData.title
               display: AbstractButton.TextUnderIcon
               down: setupBar.currentIndex == index
               font.capitalization: Font.MixedCase
               font.pixelSize: 20
               onToggled: setupBar.currentIndex = index
            }
         }
      }


      StackLayout {
         currentIndex: setupBar.currentIndex
         Layout.fillHeight: true
         Layout.fillWidth: true

         HomeView { }
         EdgeView { }
         DeviceView { }
      }
   }
}
