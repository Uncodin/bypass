#import <SenTestingKit/SenTestingKit.h>
#import "BPParser.h"

@interface BPParserTests : SenTestCase
@end

@implementation BPParserTests
{
    BPParser *parser;
}

- (void)setUp
{
    parser = [[BPParser alloc] init];
}

- (void)testSimpleCase
{
    NSString *example =
    
    @"This is a normal paragraph:\n"
    "\n"
    "    This is a code block.";
    
    BPDocument *document = [parser parse:example];
    
    STAssertNotNil(document, @"Expected a non-nil document");
    STAssertEquals([[document elements] count], 2U, @"Expected 2 elements in document");
    
    STAssertEquals([[document elements][0] elementType], BPParagraph, @"Expected first element to be a paragraph");
    STAssertEquals([[[document elements][0] childElements][0] elementType], BPText, @"Expected first element to have an inner text element");
    STAssertEqualObjects([[[document elements][0] childElements][0] text], @"This is a normal paragraph:", @"Expected first inner text element to match");
    
    STAssertEquals([[document elements][1] elementType], BPBlockCode, @"Expected second element to be a code block");
    STAssertEquals([[[document elements][1] childElements][0] elementType], BPText, @"Expected second element to have an inner text element");
    STAssertEqualObjects([[[document elements][1] childElements][0] text], @"This is a code block.", @"Expected second inner text element to match");
}

@end
