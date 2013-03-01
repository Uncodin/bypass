package in.uncod.android.bypass.test;

import java.io.InputStream;

import in.uncod.android.bypass.Bypass;
import in.uncod.andrpod.bypass.bypass_test.R;
import android.app.Activity;
import android.content.res.Resources;
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
		String readme = loadFile();
		CharSequence string = bypass.markdownToSpannable(readme);
		text.setText(string);
		//Allows link clicking
		text.setMovementMethod(LinkMovementMethod.getInstance());
	}

	private String loadFile() {
		try {
	        Resources res = getResources();
	        InputStream in_s = res.openRawResource(R.raw.readme);

	        byte[] b = new byte[in_s.available()];
	        in_s.read(b);
	        return new String(b);
	    } catch (Exception e) {
	        // e.printStackTrace();
	    }
		return "";
	}
}
