#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <MMORPGEngine/Core/World/Catalog/GroundCatalog.h>

namespace Engine {

class WorldModel {
public:
    WorldModel();

    const GroundCatalog& groundCatalog() const;
    GroundCatalog& groundCatalog();

private:
    // --- Catalogs ---
    GroundCatalog _groundCatalog;

    // Precisamos armazenar a estrutura do mundo aqui nessa classe

    /*
        Ela será usada no server e no editor
        Deve carregar toda a estrutura do mundo e mante-la em memoria
     */
};

} // namespace Engine

#endif // WORLDMODEL_H
