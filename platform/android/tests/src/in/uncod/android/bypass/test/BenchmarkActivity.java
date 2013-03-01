package in.uncod.android.bypass.test;

import in.uncod.android.bypass.Bypass;
import in.uncod.andrpod.bypass.bypass_test.R;
import android.app.Activity;
import android.os.Bundle;
import android.text.Spannable;
import android.widget.TextView;

public class BenchmarkActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.main);
		
		TextView text = (TextView) findViewById(R.id.demoText);
		Bypass bypass = new Bypass();
		Spannable spannable = (Spannable) bypass.markdownToSpannable("Header!!\n--------");
		text.setText(spannable);
	}

}
