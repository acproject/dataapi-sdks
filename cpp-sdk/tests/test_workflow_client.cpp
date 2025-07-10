#include <gtest/gtest.h>
#include "dataapi/client/WorkflowClient.h"
#include "dataapi/http/HttpClient.h"
#include "dataapi/ClientConfig.h"

class WorkflowClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        config = std::make_unique<dataapi::ClientConfig>("https://api.example.com");
        httpClient = std::make_unique<dataapi::http::HttpClient>(*config);
        workflowClient = std::make_unique<dataapi::client::WorkflowClient>(*httpClient);
    }

    std::unique_ptr<dataapi::ClientConfig> config;
    std::unique_ptr<dataapi::http::HttpClient> httpClient;
    std::unique_ptr<dataapi::client::WorkflowClient> workflowClient;
};

TEST_F(WorkflowClientTest, ConstructorTest) {
    EXPECT_NE(workflowClient, nullptr);
}

// 注意：这些测试需要实际的API连接，在实际环境中应该使用mock
// TEST_F(WorkflowClientTest, ListWorkflowsTest) {
//     auto result = workflowClient->listWorkflows(0, 10);
//     EXPECT_GE(result.size, 0);
//     EXPECT_GE(result.totalElements, 0);
// }