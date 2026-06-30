pragma Singleton

import QtQuick

QtObject {
    // Brand
    readonly property color primary: "#083B2F"
    readonly property color primaryHover: "#0B6E4F"
    readonly property color primaryPressed: "#0A5D3F"

    // Backgrounds
    readonly property color background: "#1A1A1A"
    readonly property color surface: "#E8E0C8"

    // Details
    readonly property color accent: "#D4AF37"
    readonly property color accentHover: "#E4C14E"
    readonly property color accentPressed: "#B99221"

    // Text
    readonly property color textPrimary: "#1A1A1A"
    readonly property color textSecondary: "#F0F0F0"

    // Status
    readonly property color success: "#56C271"
    readonly property color warning: "#F8B400"
    readonly property color error: "#D9534F"
}
