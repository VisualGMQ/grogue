Config = {
    totle_weight = 20,

    items = {
        stone = {
            name = "石头",
            operations = {"drop", "use"},
            weight = 2,
            sprite = {
                color = {255, 255, 255, 255},
                tilesheet = {
                    name = "tilesheet",
                    row = 9,
                    col = 2,
                },
            },
            material = {
                opaque = 1.0,
                solid = 1.0,
                lumen = 0.0,
            },
        },
        bigstone = {
            name = "大石头",
            operations = {"drop", "use"},
            weight = 4,
            sprite = {
                color = {255, 255, 255, 255},
                tilesheet = {
                    name = "tilesheet",
                    row = 9,
                    col = 1,
                },
            },
            material = {
                opaque = 1.0,
                solid = 1.0,
                lumen = 0.0,
            },
        },
    }
}