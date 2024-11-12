// clazy:excludeall=non-pod-global-static

#include <catch2/catch_all.hpp>

#include <filesystem>
#include <format>
#include <fstream>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <sstream>

using namespace std;
using namespace nlohmann::json_literals;

#define __XSTR__(s) __STR__(s)
#define __STR__(s) #s

string_view getResourcesDir() { return __XSTR__(__RESOURCES_DIR__); }

std::string getResourcePath(const std::string_view& relativePath)
{
    return std::filesystem::path(getResourcesDir()) / relativePath;
}

std::string readFile(const std::string& filePath)
{
    if (!filesystem::exists(filePath))
        throw runtime_error(format("File not found: {}", filePath));
    try {
        std::ifstream s(filePath);
        std::stringstream buffer;
        buffer << s.rdbuf();
        return buffer.str();
    } catch (const exception& e) {
        throw runtime_error(format("Cannot read file: {}, error: {}", filePath, e.what()));
    }
}

string readResource(const std::string_view& relativePath) { return readFile(getResourcePath(relativePath)); }

void formatChecker(const std::string& /*format*/, const std::string& /*value*/) {
};

TEST_CASE("Validate schema")
{
    auto wrongJson = R"(
{
    "wrong": "json"
}
)"_json;

    SECTION("Schema 3.0.x (2019-04-02)")
    {
        auto schemaStr = readResource("schema_1.json");
        auto schemaJson = nlohmann::json::parse(schemaStr);
        nlohmann::json_schema::json_validator validator(nullptr, formatChecker);

        // FIXME: Unexpected exception "type must be number, but is boolean"
        validator.set_root_schema(schemaJson);

        REQUIRE_THROWS(validator.validate(wrongJson));
    }

    SECTION("Schema 3.1.x (2020-12)")
    {
        auto schemaStr = readResource("schema_2.json");
        auto schemaJson = nlohmann::json::parse(schemaStr);
        nlohmann::json_schema::json_validator validator(nullptr, formatChecker);
        validator.set_root_schema(schemaJson);

        // FIXME: this validation is expected to throw a validation error, but it completed successfully
        REQUIRE_THROWS(validator.validate(wrongJson));
    }
}
