#import <SenTestingKit/SenTestingKit.h>
#import "BPDocumentPrivate.h"

@interface BPDocumentTests : SenTestCase
@end

@implementation BPDocumentTests
{
    BPDocument *document;
}

- (void)setUp
{
    using namespace Bypass;
    
    Document d;
    
    Element e0;
    e0.setType(PARAGRAPH);
    e0.setText("text");
    e0.addAttribute("a", "A");
    e0.addAttribute("b", "B");
    d.append(e0);

    Element e1;
    e1.setType(PARAGRAPH);
    e1.setText("text");
    e1.addAttribute("a", "A");
    e1.addAttribute("b", "B");
    d.append(e1);
    
    document = [[BPDocument alloc] initWithDocument:d];
}

- (void)testInitialization
{
    STAssertNotNil(document, @"Expected non-nil document");
}

- (void)testElementsAccessor_forPointerEquality
{
    NSArray *elements = [document elements];
    STAssertEquals([document elements], elements, @"Expected same elements");
}

- (void)testChildElements
{
    STAssertEquals([[document elements] count], 2U, @"Expected 2 elements");
    STAssertEquals([[document elements][0] elementType], BPParagraph, @"Expected first element type to be BPParagraph");
    STAssertEquals([[document elements][1] elementType], BPParagraph, @"Expected second element type to be BPParagraph");
}

#if __has_feature(objc_subscripting)

- (void)testNumericSubscripting
{
    STAssertEquals(document[0], [[document elements] objectAtIndex:0], @"Expected document subscripting to return an element");
}

#endif


@end
