
#ifndef MYSQLOO_LUAQUERY_H
#define MYSQLOO_LUAQUERY_H


#include "LuaIQuery.h"

class LuaQuery : public LuaIQuery {
public:
    int m_dataReference = 0;

    static int createDataReference(ILuaBase *LUA, Query &query, QueryData &data);

    static void addMetaTableFunctions(ILuaBase *LUA);

    static void createMetaTable(ILuaBase *LUA);

    static LuaQuery *getLuaQuery(ILuaBase *LUA, int stackPos = 1) {
        int type = LuaObject::getLuaObjectType(LUA, stackPos);
        if (type != LuaObject::TYPE_QUERY && type != LuaObject::TYPE_PREPARED_QUERY) {
            LUA->ThrowError("[MySQLOO] Expected MySQLOO query");
        }
        auto *returnValue = LuaObject::getLuaObject<LuaQuery>(LUA, type, stackPos);
        if (returnValue == nullptr) {
            LUA->ThrowError("[MySQLOO] Expected MySQLOO table");
        }
        return returnValue;
    }

    static std::shared_ptr<LuaQuery> create(const std::shared_ptr<Query> &query) {
        auto instance = std::shared_ptr<LuaQuery>(new LuaQuery(query, "MySQLOO Query"));
        LuaObject::luaObjects.push_back(instance);
        return instance;
    }

    void runSuccessCallback(ILuaBase *LUA, const std::shared_ptr<IQueryData> &data) override;

    std::shared_ptr<IQueryData> buildQueryData(ILuaBase *LUA, int stackPosition) override;

protected:
    explicit LuaQuery(const std::shared_ptr<Query> &query, const std::string &className) : LuaIQuery(
            std::dynamic_pointer_cast<IQuery>(query), className) {
    }
};

#endif //MYSQLOO_LUAQUERY_H