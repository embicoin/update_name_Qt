package org.owata_programer.update_name_Qt;

import android.app.Activity;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.widget.Toast;

class UpdateNameQtService extends Service{
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onStart(Intent intent, int startId) {
        Toast.makeText(this, "update_name_Qtサービスが起動されました。", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onDestroy() {
            Toast.makeText(this, "update_name_Qtサービスを終了しました。", Toast.LENGTH_SHORT).show();
    }
}

public class ServiceManager extends Activity{

    public void start() {
        Intent intent = new Intent(this, UpdateNameQtService.class);
        startService(intent);
    }

    public void stop() {
        Intent intent = new Intent(this, UpdateNameQtService.class);
        stopService(intent);
    }
}
