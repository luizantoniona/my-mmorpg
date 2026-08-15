CREATE TABLE account (
    id_account INTEGER PRIMARY KEY AUTOINCREMENT,
    ds_username VARCHAR NOT NULL UNIQUE,
    ds_password VARCHAR NOT NULL
);

CREATE TABLE 'character' (
    id_character INTEGER PRIMARY KEY AUTOINCREMENT,
    id_account INTEGER NOT NULL,
    ds_name VARCHAR(20) NOT NULL,

    FOREIGN KEY (id_account) REFERENCES account(id_account) ON DELETE CASCADE
);

CREATE TABLE character_inventory (
    id_character INTEGER NOT NULL,
    id_item TEXT NOT NULL,
    amount INTEGER DEFAULT 0,

    PRIMARY KEY (id_character, id_item),
    FOREIGN KEY (id_character) REFERENCES 'character'(id_character) ON DELETE CASCADE
);

CREATE TABLE character_vitals (
    id_character INTEGER PRIMARY KEY,
    health NUMERIC DEFAULT 0.0,
    max_health NUMERIC DEFAULT 0.0,
    mana NUMERIC DEFAULT 0.0,
    max_mana NUMERIC DEFAULT 0.0,
    stamina NUMERIC DEFAULT 0.0,
    max_stamina NUMERIC DEFAULT 0.0,

    FOREIGN KEY (id_character) REFERENCES 'character'(id_character) ON DELETE CASCADE
);
