# Profiling

The following profiling data was taken by running the BypassSample on an
iPhone 5 (Model A1428) using the [Time Profiler Trace Template](https://developer.apple.com/library/ios/#documentation/DeveloperTools/Conceptual/InstrumentsUserGuide/AnalysingCPUUsageinYourOSXApp/AnalysingCPUUsageinYourOSXApp.html#//apple_ref/doc/uid/TP40004652-CH16-SW8)
in Intruments

## Result

Note: I have the scaled the following images down by 50%.

![UITextView](./UITextView.png)
![UILabel](./UILabel.png)

As you can see, the results are practically identical. The only difference that
I can see is in the way the underline is rendered. In the `UITextView`, it seems
to be as close to the baseline as possible without being flush with it. The
`UILabel` example has it off a bit more and the added bonus of avoiding the
descenders. Also note that I shifted the `UITextView` down by 3 pts in order to
line up the two examples.

## Condensed View

The following condensed view of the profiling data shows you how much quicker it
is to bypass the HTML rendering system that UIKit uses to draw attributed
strings into a `UITextView`, and instead draw into a `UILabel` using the
typesetting subsystem. All of the following data was used to produce the results
above.

    Running Time	Self	Symbol Name
    99.0ms   38.2%	79.0	-[BPTextViewController viewWillAppear:]
    20.0ms    7.7%	0.0	 	-[BPMarkdownViewController viewWillAppear:]

## Expanded View

The expanded view of the data follows.

### UITextView

    Running Time	Self	Symbol Name
    99.0ms   38.2%	 0	 	-[BPTextViewController viewWillAppear:]
    78.0ms   30.1%	 0	 	 -[UITextView setAttributedText:]
    47.0ms   18.1%	13	 	  -[UITextView setContentToHTMLString:]
    32.0ms   12.3%	28	 	   -[DOMHTMLElement setInnerHTML:]
    4.0ms     1.5%	 0	 	    -[WebHTMLView setNeedsDisplayInRect:]
    4.0ms     1.5%	 0	 	     -[WAKView setNeedsDisplayInRect:]
    4.0ms     1.5%	 3	 	      -[WAKWindow setNeedsDisplayInRect:]
    1.0ms     0.3%	 0	 	       -[TileLayer setNeedsDisplayInRect:]
    1.0ms     0.3%	 1	 	        -[CALayer setNeedsDisplayInRect:]
    1.0ms     0.3%	 1	 	   -[WAKScrollView documentVisibleRect]
    1.0ms     0.3%	 0	 	   -[UITextView webViewDidChange:]
    1.0ms     0.3%	 0	 	    -[UITextInteractionAssistant(UITextInteractionAssistant_Internal) selectionView]
    1.0ms     0.3%	 1	 	     -[UIView(Hierarchy) superview]
    24.0ms    9.2%	 1	 	  -[NSHTMLWriter HTMLData]
    23.0ms    8.8%	 3	 	   -[NSHTMLWriter _generateHTML]
    7.0ms     2.7%	 0	 	    -[NSHTMLWriter _paragraphClassforParagraphStyle:range:isEmpty:isCompletelyEmpty:headerString:alignmentString:directionString:]
    3.0ms     1.1%	 3	 	     -[__NSCFString appendFormat:]
    2.0ms     0.7%	 2	 	     +[UIFont fontNamesForFamilyName:]
    2.0ms     0.7%	 2	 	     -[NSHTMLWriter _defaultValueForAttribute:range:]
    4.0ms     1.5%	 0	 	    -[NSHTMLWriter _spanClassForAttributes:inParagraphClass:spanClass:flags:]
    2.0ms     0.7%	 2	 	     -[__NSCFString deleteCharactersInRange:]
    1.0ms     0.3%	 1	 	     DYLD-STUB$$+[NSOrderedSet orderedSetWithSet:]
    1.0ms     0.3%	 1	 	     -[__NSOrderedSetM indexOfObject:]
    3.0ms     1.1%	 3	 	    -[__NSCFString appendString:]
    3.0ms     1.1%	 2	 	    -[__NSCFString appendFormat:]
    1.0ms     0.3%	 1	 	     -[NSObject respondsToSelector:]
    1.0ms     0.3%	 1	 	    -[NSPlaceholderMutableString init]
    1.0ms     0.3%	 1	 	    -[__NSCFString substringWithRange:]
    1.0ms     0.3%	 1	 	    -[__NSCFNumber unsignedIntegerValue]
    4.0ms     1.5%	 0	 	  -[NSString stringByReplacingOccurrencesOfString:withString:]
    4.0ms     1.5%	 2	 	   -[NSString stringByReplacingOccurrencesOfString:withString:options:range:]
    2.0ms     0.7%	 2	 	    -[__NSCFString replaceOccurrencesOfString:withString:options:range:]
    2.0ms     0.7%	 2	 	  -[NSPlaceholderString initWithData:encoding:]
    1.0ms     0.3%	 0	 	  -[NSMutableString(UIKBExtras) _removeOccurrencesOfCharacter:]
    1.0ms     0.3%	 1	 	   -[NSMutableString replaceOccurrencesOfString:withString:options:range:]
    13.0ms    5.0%	 0	 	 -[BPAttributedStringConverter convertDocument:]
    13.0ms    5.0%	 0	 	  -[BPAttributedStringConverter renderElement:toTarget:]
    10.0ms    3.8%	 0	 	   -[BPAttributedStringConverter renderElement:toTarget:]
    3.0ms     1.1%	 0	 	    -[BPAttributedStringConverter renderListItemElement:inRange:toTarget:]
    1.0ms     0.3%	 0	 	     -[NSMutableAttributedString addAttributes:range:]
    1.0ms     0.3%	 1 	 	      -[__NSDictionaryI objectForKey:]
    1.0ms     0.3%	 1	 	     -[NSConcreteMutableAttributedString dealloc]
    1.0ms     0.3%	 0	 	     -[BPAttributedStringConverter insertNewlineIntoTarget:]
    1.0ms     0.3%	 0	 	      -[NSConcreteMutableAttributedString initWithString:]
    1.0ms     0.3%	 0	 	       -[NSConcreteMutableAttributedString replaceCharactersInRange:withString:]
    1.0ms     0.3%	 1	 	        -[__NSCFString replaceCharactersInRange:withString:]
    3.0ms     1.1%	 0	 	    -[BPAttributedStringConverter renderTextElement:toTarget:]
    3.0ms     1.1%	 0	 	     -[BPAttributedStringConverter renderSpanElement:withFont:toTarget:]
    3.0ms     1.1%	 0	 	      -[BPAttributedStringConverter renderSpanElement:withFont:attributes:toTarget:]
    2.0ms     0.7%	 0	 	       -[BPAttributedStringConverter UIFontFromCTFont:]
    2.0ms     0.7%	 0	 	        +[UIFont fontWithName:size:]
    2.0ms     0.7%	 1	 	         +[UIFont fontWithName:size:traits:]
    1.0ms     0.3%	 0	 	          -[__NSCFString UTF8String]
    1.0ms     0.3%	 0	 	           -[NSString(NSStringOtherEncodings) UTF8String]
    1.0ms     0.3%	 1	 	            -[NSString(NSStringOtherEncodings) getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]
    1.0ms     0.3%	 1	 	       -[NSConcreteAttributedString initWithString:attributes:]
    2.0ms     0.7%	 0	 	    -[BPAttributedStringConverter renderElement:toTarget:]
    2.0ms     0.7%	 0	 	     -[BPAttributedStringConverter renderTextElement:toTarget:]
    2.0ms     0.7%	 0	 	      -[BPAttributedStringConverter renderSpanElement:withFont:toTarget:]
    2.0ms     0.7%	 1	 	       -[BPAttributedStringConverter renderSpanElement:withFont:attributes:toTarget:]
    1.0ms     0.3%	 1	 	        -[__NSDictionaryM setObject:forKey:]
    2.0ms     0.7%	 1	 	    -[BPElement childElements]
    1.0ms     0.3%	 1	 	     +[NSArray arrayWithArray:]
    1.0ms     0.3%	 1 	 	   -[BPAttributedStringConverter insertNewlineIntoTarget:]
    1.0ms     0.3%	 0	 	   -[BPAttributedStringConverter renderBlockCodeElement:inRange:toTarget:]
    1.0ms     0.3%	 0	 	    -[NSConcreteMutableAttributedString dealloc]
    1.0ms     0.3%	 1	 	     -[NSRLEArray dealloc]
    1.0ms     0.3%	 1	 	   -[BPElement childElements]
    6.0ms     2.3%	 6	 	 -[BPParser parse:]
    1.0ms     0.3%	 0	 	 -[BPAbstractViewController sampleMarkdown]
    1.0ms     0.3%	 1	 	  -[NSBundle pathForResource:ofType:]
    1.0ms     0.3%	 1	 	 -[NSHTMLWriter dealloc]

### UILabel

    Running Time	Self	Symbol Name
    20.0ms    7.7%	 0	 	-[BPMarkdownViewController viewWillAppear:]
    18.0ms    6.9%	10	 	 -[BPDocumentRenderer renderDocument:suggestedContainerSize:]
    7.0ms     2.7%	 0	 	  -[BPAttributedStringConverter convertDocument:]
    7.0ms     2.7%	 0	 	   -[BPAttributedStringConverter renderElement:toTarget:]
    4.0ms     1.5%	 0	 	    -[BPAttributedStringConverter renderElement:toTarget:]
    3.0ms     1.1%	 0	 	     -[BPAttributedStringConverter renderElement:toTarget:]
    2.0ms     0.7%	 0	 	      -[BPAttributedStringConverter renderTextElement:toTarget:]
    1.0ms     0.3%	 0	 	       -[BPAttributedStringConverter renderSpanElement:withFont:toTarget:]
    1.0ms     0.3%	 0	 	        -[BPAttributedStringConverter renderSpanElement:withFont:attributes:toTarget:]
    1.0ms     0.3%	 0	 	         -[BPAttributedStringConverter UIFontFromCTFont:]
    1.0ms     0.3%	 0	 	          +[UIFont fontWithName:size:]
    1.0ms     0.3%	 0	 	           +[UIFont fontWithName:size:traits:]
    1.0ms     0.3%	 0	 	            -[__NSCFString UTF8String]
    1.0ms     0.3%	 1	 	             -[NSString(NSStringOtherEncodings) UTF8String]
    1.0ms     0.3%	 1	 	       -[__NSDictionaryM dealloc]
    1.0ms     0.3%	 0	 	      -[BPAttributedStringConverter renderElement:toTarget:]
    1.0ms     0.3%	 0	 	       -[BPAttributedStringConverter insertBulletIntoTarget:]
    1.0ms     0.3%	 1	 	        -[NSConcreteMutableAttributedString initWithString:attributes:]
    1.0ms     0.3%	 0	 	     -[BPAttributedStringConverter renderParagraphElement:inRange:toTarget:]
    1.0ms     0.3%	 0	 	      -[NSMutableAttributedString addAttributes:range:]
    1.0ms     0.3%	 0	 	       -[NSAttributeDictionary mutableCopyWithZone:]
    1.0ms     0.3%	 1	 	        -[__NSPlaceholderDictionary initWithObjects:forKeys:count:]
    2.0ms     0.7%	 2	 	    -[BPElement childElements]
    1.0ms     0.3%	 0	 	    -[NSConcreteMutableAttributedString dealloc]
    1.0ms     0.3%	 1	 	     -[NSRLEArray dealloc]
    1.0ms     0.3%	 1	 	  -[NSDictionary __apply:context:]
    2.0ms     0.7%	 2	 	 -[BPParser parse:]


