/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>

#include "logger.h"
#include "database.h"
#include "exceptions.h"

namespace ddb{

// Initialize spatialite
void Database::Initialize() {
    spatialite_init (0);
}

Database &Database::createTables() {
    std::string sql = R"<<<(
  SELECT InitSpatialMetaData(1, 'NONE');
  SELECT InsertEpsgSrid(4326);

  CREATE TABLE IF NOT EXISTS entries (
      path TEXT PRIMARY KEY,
      hash TEXT,
      type INTEGER,
      meta TEXT,
      mtime INTEGER,
      size  INTEGER,
      depth INTEGER
  );
  SELECT AddGeometryColumn("entries", "point_geom", 4326, "POINTZ", "XYZ");
  SELECT AddGeometryColumn("entries", "polygon_geom", 4326, "POLYGONZ", "XYZ");

  CREATE TABLE IF NOT EXISTS passwords (
      salt TEXT,
      hash TEXT      
  );

  CREATE TABLE IF NOT EXISTS attributes (
      name TEXT NOT NULL PRIMARY KEY,
      ivalue INTEGER,
      rvalue REAL,
      tvalue TEXT,
      bvalue BLOB
  );

)<<<";

    LOGD << "About to create tables...";
    this->exec(sql);
    LOGD << "Created tables";

    return *this;
}

void Database::setPublic(bool isPublic){
    this->setBoolAttribute("public", isPublic);
}

bool Database::isPublic() const{
    if (this->hasAttribute("public")) return this->getBoolAttribute("public");
    else return false;
}

void Database::chattr(json attrs){
    for (auto& el : attrs.items()) {
        if (el.key() == "public" && el.value().is_boolean()){
            this->setBoolAttribute("public", el.value());
        }else{
            throw InvalidArgsException("Invalid attribute " + el.key());
        }
    }
}

json Database::getAttributes() const{
    json j;
    j["public"] = this->isPublic();
    return j;
}

void Database::setBoolAttribute(const std::string &name, bool value){
    this->setIntAttribute(name, value ? 1 : 0);
}

bool Database::getBoolAttribute(const std::string &name) const{
    return this->getIntAttribute(name) == 1;
}

void Database::setIntAttribute(const std::string &name, int value){
    const std::string sql = "INSERT OR REPLACE INTO attributes (name, ivalue) "
                            "VALUES(?, ?)";

    const auto q = this->query(sql);

    q->bind(1, name);
    q->bind(2, value);

    q->execute();
}

int Database::getIntAttribute(const std::string &name) const{
    const std::string sql = "SELECT ivalue FROM attributes WHERE name = ?";

    const auto q = this->query(sql);
    q->bind(1, name);
    if (q->fetch()) return q->getInt(0);
    else return 0;
}

bool Database::hasAttribute(const std::string &name) const{
    const std::string sql = "SELECT COUNT(name) FROM attributes WHERE name = ?";

    const auto q = this->query(sql);
    q->bind(1, name);
    q->fetch();

    return q->getInt(0) == 1;
}

void Database::clearAttribute(const std::string &name){
    const std::string sql = "DELETE FROM attributes WHERE name = ?";

    const auto q = this->query(sql);
    q->bind(1, name);
    q->execute();
}

}
