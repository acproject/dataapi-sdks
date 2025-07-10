#include <gtest/gtest.h>
#include "dataapi/http/HttpClient.h"
#include "dataapi/ClientConfig.h"

class HttpClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        config = std::make_unique<dataapi::ClientConfig>("https://api.example.com");
        httpClient = std::make_unique<dataapi::http::HttpClient>(*config);
    }

    std::unique_ptr<dataapi::ClientConfig> config;
    std::unique_ptr<dataapi::http::HttpClient> httpClient;
};

TEST_F(HttpClientTest, ConstructorTest) {
    EXPECT_NE(httpClient, nullptr);
}

// 注意：这些测试需要实际的网络连接，在实际环境中应该使用mock
// TEST_F(HttpClientTest, GetRequestTest) {
//     auto response = httpClient->get("/health");
//     EXPECT_GE(response.statusCode, 100);
//     EXPECT_LE(response.statusCode, 599);
// }