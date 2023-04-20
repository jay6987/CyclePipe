#include <gtest/gtest.h>
#include "../CyclePipe/SyncBase.h"

using namespace CyclePipe;

struct SyncClassClosedAndEmptySignal: SyncObjectClosedAndEmptySignal{};
class SyncClass: public SyncBase
{
public:
    bool GetSomething()
    {
        if (Closed())
            throw SyncClassClosedAndEmptySignal();
        return true;
    }
};

TEST(SyncBaseTest, Close)
{
    SyncClass syncObj;
    EXPECT_TRUE(syncObj.GetSomething());

    syncObj.Close();
    EXPECT_THROW(
        syncObj.GetSomething()
        , SyncClassClosedAndEmptySignal
    );
}
