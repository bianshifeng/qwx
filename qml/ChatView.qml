// Copyright (C) 2014 - 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.2
import QtGraphicalEffects 1.0
import cn.com.isoft.qwx 1.0
import "global.js" as Global

Rectangle {
	id: chatView
	color: "white"

	property string fromUserName
	property string toUserName
	property string toNickName
	property bool showEmotion: false
	property var toUserList

	function moveToTheEnd() {
		chatListView.positionViewAtEnd()
	}

	Component.onCompleted: {
		Global.chatView = chatView
	}

	Clipboard {
		id: clipboard
	}

	XiaoDouBi {
		id: xiaodoubiObj
		onContentChanged: {
			if (!Global.isRobot) {
				return
			}
			if (content == "") {
				return
			}
			if (Global.v2) {
				sendMsgObj.sendV2(Global.uin, Global.sid, Global.skey,
								  Global.deviceId, chatView.fromUserName,
								  chatView.toUserName, content, Global.syncKey)
			} else {
				sendMsgObj.send(Global.uin, Global.sid, Global.skey,
								Global.deviceId, chatView.fromUserName,
								chatView.toUserName, content, Global.syncKey)
			}
			chatListModel.append({
									 content: content,
									 curUserName: chatView.fromUserName
								 })
			moveToTheEnd()
		}
	}

	function doNewMsg() {
		if (Global.v2) {
			getMsgObj.postV2(Global.uin, Global.sid, Global.skey,
							 Global.syncKey)
		} else {
			getMsgObj.post(Global.uin, Global.sid, Global.skey, Global.syncKey)
		}
	}

	GetMsg {
		id: getMsgObj
		fromUserName: chatView.fromUserName
		toUserName: chatView.toUserName
		needSaveLog: false
		onNoNewMsg: {
			rootWindow.title = qsTr("WeChat Qt frontend")
		}
		onReceived: {
			rootWindow.title = qsTr("WeChat Qt frontend") + " - " + qsTr(
						"New message")
			if (content == "小逗比退下" || content == "robot away") {
				Global.isRobot = false
			} else if (content == "小逗比出来" || content == "robot come") {
				Global.isRobot = true
			}
			var newContent = getMsgObj.contentWithoutUserName(content)
			var newUserName = getMsgObj.contentToUserName(content, userName)
			chatListModel.append({
									 content: newContent,
									 curUserName: newUserName
								 })
			moveToTheEnd()
			if (Global.isRobot) {
				xiaodoubiObj.get(content)
			}
		}
		onNewMsg: {
			if (Global.isRobot) {
				xiaodoubiObj.get(content)
			}
		}
	}

	SendMsg {
		id: sendMsgObj
	}

	ListModel {
		id: chatListModel
		Component.onCompleted: {
			chatListModel.remove(0)
		}

		ListElement {
			content: ""
			curUserName: ""
		}
	}

	ChatLog {
		id: chatLogObj
		Component.onCompleted: {
			chatLogObj.load(chatView.toUserName)
		}
		onChatLogListChanged: {
			for (var i = 0; i < chatLogObj.chatLogList.length; i++) {
				chatListModel.append({
										 curUserName: chatLogObj.chatLogList[i].userName,
										 content: chatLogObj.chatLogList[i].content
									 })
			}
			moveToTheEnd()
		}
	}

	ListView {
		id: chatListView
		model: chatListModel
		width: parent.width
		anchors.top: chatHeader.bottom
		anchors.bottom: inputRect.top
		spacing: 0
		delegate: Item {

			width: parent.width
			height: (chatText.contentHeight < fromUserHeadImage.height ? fromUserHeadImage.height : chatText.contentHeight) + 12

			HeadImg {
				id: fromUserHeadImgObj
				userName: curUserName
				onFilePathChanged: {
					fromUserHeadImage.imageSource = fromUserHeadImgObj.filePath
				}
			}

			CircleImage {
				id: fromUserHeadImage
				width: 42
				height: 42
				x: curUserName
				   !== fromUserName ? Global.styles.navbar.horizontalMargin : parent.x
									  + parent.width - Global.styles.navbar.horizontalMargin - width
				anchors.verticalCenter: parent.verticalCenter
			}

			Text {
				id: chatText
				horizontalAlignment: curUserName != fromUserName ? Text.AlignLeft : Text.AlignRight
				text: emotionObj.contentWithImage(content)
				wrapMode: Text.WordWrap
				textFormat: Text.StyledText
				font.pixelSize: 11
				width: parent.width - fromUserHeadImage.width
					   - Global.styles.navbar.horizontalMargin
					   - Global.styles.navbar.horizontalMargin - 8
				x: curUserName === fromUserName ? Global.styles.navbar.horizontalMargin : (parent.x + parent.width - Global.styles.navbar.horizontalMargin - width)
				anchors.verticalCenter: parent.verticalCenter

				MouseArea {
					anchors.fill: parent
					onPressed: {
						clipboard.copy(content)
					}
				}
			}
			Rectangle {
				anchors.bottom: parent.bottom
				width: parent.width
				height: 1
				color: "#dadada"
			}
		}
	}

	Rectangle {
		id: chatHeader
		width: parent.width
		height: 58
		anchors.top: parent.top
		color: "#20282a"

		Rectangle {
			id: backImage
			color: "#00000000"
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.topMargin: 6
			anchors.bottomMargin: 6
			anchors.left: parent.left
			anchors.leftMargin: Global.styles.navbar.horizontalMargin
			width: height
			radius: 4

			Image {
				anchors.fill: parent
				source: "../images/back.png"
			}

			MouseArea {
				hoverEnabled: true
				anchors.fill: parent
				onClicked: {
					navigatorStackView.pop()
				}
				onPressed: {
					backImage.color = "#8844ff44"
				}
				onReleased: {
					backImage.color = "#5544ff44"
				}

				onEntered: {
					backImage.color = "#5544ff44"
				}
				onExited: {
					backImage.color = "#0044ff44"
				}
			}
		}

		Text {
			text: chatView.toNickName
			font.pixelSize: 22
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: backImage.right
			anchors.leftMargin: 19
			anchors.right: participants.right
			anchors.rightMargin: 8
			antialiasing: true
			color: "white"
		}

		CircleImage {
			id: participants
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.topMargin: 6
			anchors.bottomMargin: 6
			width: height
			anchors.rightMargin: Global.styles.navbar.horizontalMargin
			imageSource: participantsImage
			color: "#00000000"

			HeadImg {
				id: participantsImage
				userName: toUserName
				onFilePathChanged: {
					participants.imageSource = participantsImage.filePath
				}
			}
		}
	}

	function sendMsg() {
		if (sendMsgTextField.text == "") {
			return
		}
		sendMsgTextField.text = sendMsgTextField.text.replace(
					"qwx",
					"消息发自 qwx - 微信 Qt 前端 https://github.com/xiangzhai/qwx")
		if (typeof (chatView.toUserList) == 'undefined') {
			if (Global.v2) {
				sendMsgObj.sendV2(Global.uin, Global.sid, Global.skey,
								  Global.deviceId, chatView.fromUserName,
								  chatView.toUserName, sendMsgTextField.text,
								  Global.syncKey)
			} else {
				sendMsgObj.send(Global.uin, Global.sid, Global.skey,
								Global.deviceId, chatView.fromUserName,
								chatView.toUserName, sendMsgTextField.text,
								Global.syncKey)
			}
		} else {
			for (var i = 0; i < chatView.toUserList.length; i++) {
				if (Global.v2) {
					sendMsgObj.sendV2(Global.uin, Global.sid, Global.skey,
									  Global.deviceId, chatView.fromUserName,
									  chatView.toUserList[i].userName,
									  sendMsgTextField.text, Global.syncKey);
				} else {
					sendMsgObj.send(Global.uin, Global.sid, Global.skey,
									Global.deviceId, chatView.fromUserName,
									chatView.toUserList[i].userName,
									sendMsgTextField.text, Global.syncKey);
                }
                sleep(3000);
			}
		}
		chatListModel.append({
								 content: sendMsgTextField.text,
								 curUserName: chatView.fromUserName
							 })
		if (sendMsgTextField.text == "away") {
			Global.isAway = true
		} else if (sendMsgTextField.text == "back") {
			Global.isAway = false
		} else if (sendMsgTextField.text == "小逗比出来"
				   || sendMsgTextField.text == "robot come") {
			Global.isRobot = true
			xiaodoubiObj.get(sendMsgTextField.text)
		} else if (sendMsgTextField.text == "小逗比退下"
				   || sendMsgTextField.text == "robot away") {
			Global.isRobot = false
		} else if (Global.isRobot) {
			xiaodoubiObj.get(sendMsgTextField.text)
		}

		sendMsgTextField.text = ""
		moveToTheEnd()
	}

	Emotion {
		id: emotionObj
	}

	GridView {
		id: emotionGridView
		model: emotionObj
		width: chatView.width
		height: 168
		anchors.bottom: inputRect.top
		cellWidth: 40
		cellHeight: 40
		anchors.horizontalCenter: parent.horizontalCenter
		focus: true
		visible: false
		clip: true

		delegate: Item {
			width: parent.width
			height: parent.height

			Rectangle {
				anchors.fill: parent
				color: "white"

				//					border.color: "black"
				//					border.width: 3
				AnimatedImage {
					source: "../images/" + name
					width: 24
					height: 24
					anchors.left: parent.left
					anchors.leftMargin: 8
					anchors.top: parent.top
					anchors.topMargin: 8

					MouseArea {
						anchors.fill: parent
						onClicked: {
							sendMsgTextField.text += "[" + title + "]"
						}
					}
				}
			}
		}
		z: 1
	}

	RectangularGlow {
		anchors.fill: emotionGridView
		visible: emotionGridView.visible
		anchors.margins: -0
		glowRadius: 12
		cornerRadius: -12
		spread: -0.3
		color: "black"
	}

	Rectangle {
		z: 1
		height: 36
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		color: "white"
		id: inputRect

		Item {
			anchors.fill: parent
			anchors.margins: 6

			TextField {
				id: sendMsgTextField
				anchors.left: parent.left
				anchors.right: emotionImage.left
				anchors.verticalCenter: parent.verticalCenter
				anchors.rightMargin: 4
				onAccepted: {
					sendMsg()
				}
			}

			AnimatedImage {
				id: emotionImage
				width: 24
				anchors.rightMargin: 4
				height: parent.height
				source: "../images/icon_smile.gif"
				anchors.right: sendButton.left
				anchors.verticalCenter: parent.verticalCenter

				MouseArea {
					anchors.fill: parent
					onClicked: {
						chatView.showEmotion = !chatView.showEmotion
						emotionGridView.visible = chatView.showEmotion
					}
				}
			}

			Button {
				id: sendButton
				text: qsTr("Send")
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter
				onClicked: {
					emotionGridView.visible = false
					sendMsg()
				}
			}
		}

		RectangularGlow {
			z: -1
			anchors.fill: parent
			glowRadius: 12
			cornerRadius: -12
			spread: -0.3
			color: "black"
		}
	}
}
