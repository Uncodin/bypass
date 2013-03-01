package in.uncod.android.bypass.test;

import in.uncod.android.bypass.Bypass;
import in.uncod.andrpod.bypass.bypass_test.R;
import android.app.Activity;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.widget.TextView;

public class BenchmarkActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.main);
		
		TextView text = (TextView) findViewById(R.id.demoText);
		Bypass bypass = new Bypass();
		CharSequence string = bypass.markdownToSpannable("Header!!\n--------\nText goes here or something\n*One* **Two** ***Three***\n[Link](https://github.com/Uncodin/bypass)\n\n-\tOne\n-\tTwo\n\n");
		text.setText(string);
		//Allows link clicking
		text.setMovementMethod(LinkMovementMethod.getInstance());
	}

}
