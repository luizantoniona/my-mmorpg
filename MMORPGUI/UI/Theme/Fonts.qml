pragma Singleton

import QtQuick

QtObject {
    readonly property string familyDefault: "Arial"
    readonly property string familyTitle: "Arial Black"

    readonly property font titleLarge: Qt.font({
                                                   "family": familyTitle,
                                                   "pixelSize": 24,
                                                   "bold": true,
                                                   "weight": Font.Bold
                                               })

    readonly property font titleMedium: Qt.font({
                                                    "family": familyTitle,
                                                    "pixelSize": 18,
                                                    "bold": true,
                                                    "weight": Font.DemiBold
                                                })

    readonly property font bodyDefault: Qt.font({
                                                    "family": familyDefault,
                                                    "pixelSize": 13,
                                                    "bold": false,
                                                    "weight": Font.Normal
                                                })

    readonly property font bodyBold: Qt.font({
                                                 "family": familyDefault,
                                                 "pixelSize": 13,
                                                 "bold": true,
                                                 "weight": Font.Bold
                                             })

    readonly property font caption: Qt.font({
                                                "family": familyDefault,
                                                "pixelSize": 11,
                                                "bold": false,
                                                "weight": Font.Light
                                            })
}
