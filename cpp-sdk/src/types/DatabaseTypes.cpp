#include "dataapi/types/DatabaseTypes.h"
#include <chrono>
#include <cstdint>

namespace dataapi {

// DatabaseConnectionResult JSON serialization
void to_json(Json& j, const DatabaseConnectionResult& d) {
    j = Json{
        {"success", d.success},
        {"message", d.message}
    };
    if (d.details) j["details"] = *d.details;
}

void from_json(const Json& j, DatabaseConnectionResult& d) {
    j.at("success").get_to(d.success);
    j.at("message").get_to(d.message);
    
    if (j.contains("details") && !j["details"].is_null()) {
        d.details = j["details"];
    }
}

// QueryResult JSON serialization
void to_json(Json& j, const QueryResult& q) {
    j = Json{
        {"rows", q.rows},
        {"columns", q.columns},
        {"totalRows", q.totalRows},
        {"metadata", q.metadata}
    };
}

void from_json(const Json& j, QueryResult& q) {
    j.at("rows").get_to(q.rows);
    j.at("columns").get_to(q.columns);
    j.at("totalRows").get_to(q.totalRows);
    j.at("metadata").get_to(q.metadata);
}

// SqlExecutionResult JSON serialization
void to_json(Json& j, const SqlExecutionResult& s) {
    j = Json{{"success", s.success}};
    if (s.rowsAffected) j["rowsAffected"] = *s.rowsAffected;
    if (s.data) j["data"] = *s.data;
    if (s.columns) j["columns"] = *s.columns;
    if (s.executionTime) j["executionTime"] = *s.executionTime;
    if (s.message) j["message"] = *s.message;
}

void from_json(const Json& j, SqlExecutionResult& s) {
    j.at("success").get_to(s.success);
    
    if (j.contains("rowsAffected") && !j["rowsAffected"].is_null()) {
        s.rowsAffected = j["rowsAffected"].get<int>();
    }
    if (j.contains("data") && !j["data"].is_null()) {
        s.data = j["data"].get<std::vector<Json>>();
    }
    if (j.contains("columns") && !j["columns"].is_null()) {
        s.columns = j["columns"].get<std::vector<std::string>>();
    }
    if (j.contains("executionTime") && !j["executionTime"].is_null()) {
        s.executionTime = j["executionTime"].get<int>();
    }
    if (j.contains("message") && !j["message"].is_null()) {
        s.message = j["message"].get<std::string>();
    }
}

// DatabaseConfig JSON serialization
void to_json(Json& j, const DatabaseConfig& d) {
    j = Json{
        {"type", d.type},
        {"host", d.host},
        {"port", d.port},
        {"database", d.database},
        {"username", d.username},
        {"password", d.password},
        {"options", d.options}
    };
}

void from_json(const Json& j, DatabaseConfig& d) {
    j.at("type").get_to(d.type);
    j.at("host").get_to(d.host);
    j.at("port").get_to(d.port);
    j.at("database").get_to(d.database);
    j.at("username").get_to(d.username);
    j.at("password").get_to(d.password);
    j.at("options").get_to(d.options);
}

// DatabaseInfo JSON serialization
void to_json(Json& j, const DatabaseInfo& d) {
    j = Json{
        {"id", d.id},
        {"name", d.name},
        {"type", d.type},
        {"status", d.status},
        {"createdAt", std::chrono::duration_cast<std::chrono::milliseconds>(d.createdAt.time_since_epoch()).count()}
    };
}

void from_json(const Json& j, DatabaseInfo& d) {
    j.at("id").get_to(d.id);
    j.at("name").get_to(d.name);
    j.at("type").get_to(d.type);
    j.at("status").get_to(d.status);
    
    if (j.contains("createdAt")) {
        auto createdAtMs = j.at("createdAt").get<int64_t>();
        d.createdAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(createdAtMs));
    }
}

} // namespace dataapi