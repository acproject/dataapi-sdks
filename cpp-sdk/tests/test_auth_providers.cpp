#include <gtest/gtest.h>
#include "dataapi/auth/ApiKeyAuthProvider.h"
#include "dataapi/auth/BearerTokenAuthProvider.h"
#include "dataapi/auth/BasicAuthProvider.h"

TEST(ApiKeyAuthProviderTest, GetAuthHeaders) {
    dataapi::auth::ApiKeyAuthProvider provider("test-api-key", "X-API-Key");
    auto headers = provider.getAuthHeaders();
    
    EXPECT_EQ(headers.size(), 1);
    EXPECT_EQ(headers["X-API-Key"], "test-api-key");
    EXPECT_EQ(provider.getType(), dataapi::AuthenticationType::API_KEY);
}

TEST(BearerTokenAuthProviderTest, GetAuthHeaders) {
    dataapi::auth::BearerTokenAuthProvider provider("test-token");
    auto headers = provider.getAuthHeaders();
    
    EXPECT_EQ(headers.size(), 1);
    EXPECT_EQ(headers["Authorization"], "Bearer test-token");
    EXPECT_EQ(provider.getType(), dataapi::AuthenticationType::BEARER_TOKEN);
}

TEST(BasicAuthProviderTest, GetAuthHeaders) {
    dataapi::auth::BasicAuthProvider provider("username", "password");
    auto headers = provider.getAuthHeaders();
    
    EXPECT_EQ(headers.size(), 1);
    EXPECT_TRUE(headers["Authorization"].find("Basic ") == 0);
    EXPECT_EQ(provider.getType(), dataapi::AuthenticationType::BASIC);
}