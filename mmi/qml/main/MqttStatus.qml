import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Common
import HostSecure

Pane {
   id: root
   padding: 30
   background: null

   property var mqttStatusTable: [
      { title: "Client ID",        value: HSC.mmiMqttClient.clientId        },
      { title: "Hostname",         value: HSC.mmiMqttClient.hostname        },
      { title: "Port",             value: HSC.mmiMqttClient.port            },
      { title: "Keep alive",       value: HSC.mmiMqttClient.keepAlive       },
      { title: "Protocol version", value: HSC.mmiMqttClient.protocolVersionString },
      { title: "State",            value: HSC.mmiMqttClient.stateString     },
      { title: "Error",            value: HSC.mmiMqttClient.errorString     },
      { title: "Username",         value: HSC.mmiMqttClient.username        },
      { title: "Password",         value: HSC.mmiMqttClient.password        },
      { title: "Clean session",    value: HSC.mmiMqttClient.cleanSession    },
      { title: "Will topic",       value: HSC.mmiMqttClient.willTopic       },
      { title: "Will message",     value: HSC.mmiMqttClient.willMessage     },
      { title: "Will QoS",         value: HSC.mmiMqttClient.willQoS         },
      { title: "Will retain",      value: HSC.mmiMqttClient.willRetain      },
      { title: "Auto keep alive",  value: HSC.mmiMqttClient.autoKeepAlive   },
   ]

   TransparentPane {
      radius: 5

      RowLayout {

         ColumnLayout {
            spacing: 10

            Label {
               text: "MQTT status"
               font.pixelSize: 20
               font.bold: true
            }

            GridLayout {
               rows: titleRepeater.count
               flow: GridLayout.TopToBottom

               Repeater {
                  id: titleRepeater
                  model: root.mqttStatusTable
                  delegate: Label {
                     text: modelData.title
                  }
               }

               Repeater {
                  model: root.mqttStatusTable
                  delegate: Label {
                     text: ":"
                  }
               }

               Repeater {
                  model: root.mqttStatusTable
                  delegate: Label {
                     text: modelData.value
                     font.bold: true
                  }
               }
            }
         }

         Button {
            Layout.alignment: Qt.AlignBottom
            text: HSC.mmiMqttClient.state === MmiMqttClient.Connected ? "Disconnect" : "Connect"
            onClicked: {
               if ( HSC.mmiMqttClient.state === MmiMqttClient.Connected ) {
                  HSC.mmiMqttClient.disconnectFromHost()
               } else {
                  HSC.mmiMqttClient.connectToHost()
               }
            }
         }
      }
   }
}
