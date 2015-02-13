package org.thunderatz.tiago.thundertrekking;

import org.thunderatz.tiago.thundertrekking.util.SystemUiHider;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
/*
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
*/
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.provider.Settings;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.net.NetworkInterface;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.List;

public class MainActivity extends Activity implements SensorEventListener {

    private static final float low_pass_alpha = 0.85f;
    private TextView log;
    private SensorManager mSensorManager;
    private float[] gravity = new float[] {0.f, 0.f, 0.f};
    private Camera camera;
    private ServerThread gps;
    private ServerThread compass;
    private ServerThread proximity;
    private ServerThread torch;
    /*
    private static CameraDevice cameraDevice;
    private static CaptureRequest.Builder cameraBuilder;

    private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(CameraDevice d) {
            cameraDevice = d;
            log.append("Câmera conectada\n");
        }
        @Override
        public void onDisconnected(CameraDevice d) {
            d.close();
            cameraDevice = null;
            log.append("Câmera desconectada\n");
        }
        @Override
        public void onError(CameraDevice d, int error) {
            d.close();
            cameraDevice = null;
            log.append("Camera: erro " + Integer.toString(error) + "\n");
        }
    };
    */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        final LocationManager location = (LocationManager) getSystemService(LOCATION_SERVICE);

        if (!location.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
            startActivity(intent);
        }

        log = (TextView) findViewById(R.id.log);
        log.setMovementMethod(new ScrollingMovementMethod());
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        try {
            Enumeration<NetworkInterface> enumNetworkInterfaces = NetworkInterface.getNetworkInterfaces();
            while (enumNetworkInterfaces.hasMoreElements()) {
                NetworkInterface networkInterface = enumNetworkInterfaces.nextElement();
                Enumeration<InetAddress> enumInetAddress = networkInterface.getInetAddresses();
                while (enumInetAddress.hasMoreElements()) {
                    InetAddress inetAddress = enumInetAddress.nextElement();
                    log.append("if " + networkInterface.getName() + " (" + networkInterface.getDisplayName() + "): ");
                    if (inetAddress.isLoopbackAddress())
                        log.append("LOOPBACK ");
                    log.append(inetAddress.getHostAddress() + "\n");
                }
            }
        } catch (SocketException e) {
            log.append("NetworkInterface.getNetworkInterfaces(): " + e.toString() + "\n");
        }

        List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
        log.append("Sensores:\n");
        for (Sensor sensor : sensors)
            log.append(sensor.getName() + " (" + sensor.getVendor() + ")\n");

        //gps = new ServerThread(log, 1414, "gps");
        compass = new ServerThread(log, 1415, "compass", new ListenerRegisterer() {
            @Override
            public void register(boolean enable) {
                
            }
        });
        //proximity = new ServerThread(log, 1416, "proximity");
        torch = new ServerThread(log, 1417, "torch", null);
        /*
        PowerManager powerManager = (PowerManager) getSystemService(POWER_SERVICE);
        WakeLock wakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Sensores");
        wakeLock.acquire();
        */
        //  wakelock.release() quando não houver ninguém conectado
    }

    @Override
    protected void onResume() {
        boolean acelerometro_necessario = false;
        super.onResume();
        // TYPE_ROTATION_VECTOR retorna rotação como mix do campo magnético e giroscópio
        // (usando campo magnético para leitura da rotação, mas calculando com giroscópio a rotação
        // entre as amostras do campo magnético e permitindo maior frequência de atualização que apenas
        // com campo magnético) e será nosso sensor preferido. Tem maior consumo de bateria também
        if (mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR), SensorManager.SENSOR_DELAY_FASTEST)) {
            log.append("Com TYPE_ROTATION_VECTOR\n");
        } else {
            // Sem giroscópio
            log.append("Sem TYPE_ROTATION_VECTOR\n");
            if (mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_GEOMAGNETIC_ROTATION_VECTOR), SensorManager.SENSOR_DELAY_FASTEST)) {
                // Com leitura do campo magnético terrestre
                log.append("Com TYPE_GEOMAGNETIC_ROTATION_VECTOR\n");
            } else {
                log.append("Sem TYPE_GEOMAGNETIC_ROTATION_VECTOR\n");
                if (mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_FASTEST)) {
                    log.append("Com TYPE_MAGNETIC_FIELD\n");
                    acelerometro_necessario = true;
                } else {
                    log.append("Sem TYPE_MAGNETIC_FIELD\n");
                }
            }
        }
        if (acelerometro_necessario)
            if (mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_FASTEST))
                log.append("Com TYPE_ACCELEROMETER\n");
            else
                log.append("Sem TYPE_ACCELEROMETER\n");

        if (getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA_FLASH)) {
            String cameraId;
            log.append("Com FEATURE_CAMERA_FLASH\n");
            camera = Camera.open();
            Camera.Parameters p = camera.getParameters();
            p.setFlashMode(Camera.Parameters.FLASH_MODE_TORCH);
            camera.setParameters(p);
            camera.startPreview();
            /*
            // Ver exemplos:
            // https://github.com/googlesamples/android-Camera2Basic/blob/master/Application/src/main/java/com/example/android/camera2basic/Camera2BasicFragment.java
            // http://blog.csdn.net/torvalbill/article/details/40376145
            CameraManager manager = (CameraManager) getSystemService(Context.CAMERA_SERVICE);
            for (cameraId : manager.getCameraIdList()) {
                CameraCharacteristics characteristics = manager.getCameraCharacteristics(cameraId);

                if (characteristics.get(CameraCharacteristics.LENS_FACING) != CameraCharacteristics.LENS_FACING_FRONT)
                    break;
            }
            log.append("Abrindo câmera " + cameraId + "\n");
            manager.openCamera(cameraId, mStateCallback, null);
            */
            //cameraBuilder.set(SCALER_CROP_REGION, TORCH);
        } else
            log.append("Sem FEATURE_CAMERA_FLASH\n");
    }

    @Override
    protected void onPause() {
        super.onPause();
        // Parar de receber leituras de sensores para economizar bateria
        /* mSensorManager.unregisterListener(this); */
        // Parar câmera
        /*
        Camera.Parameters p = camera.getParameters();
        p.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
        camera.setParameters(p);
        camera.stopPreview();
        camera.release();
        */
        /*
        if (cameraDevice != null) {
            cameraDevice.close();
            cameraDevice = null;
        }
        */
    }

    @Override
    protected void onDestroy() {
        gps.close();
        compass.close();
        proximity.close();
        torch.close();
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        int i;

        switch (event.sensor.getType()) {
            case Sensor.TYPE_ACCELEROMETER: {
                boolean vazio;
                vazio = true;
                for (float n : gravity)
                    if (n != 0.) {
                        vazio = false;
                        break;
                    }

                if (vazio)
                    gravity = Arrays.copyOf(event.values, 3);
                else
                    // filtro low-pass
                    for (i = 0; i < 3; i++)
                        gravity[i] = low_pass_alpha * gravity[i] + (1.f - low_pass_alpha) * event.values[i];

                break;
            }

            case Sensor.TYPE_ROTATION_VECTOR:
            case Sensor.TYPE_GEOMAGNETIC_ROTATION_VECTOR: {
                // Isso nunca foi testado, não tenho celular com esses sensores.
                // Para quem quiser usar:
                // http://developer.android.com/reference/android/hardware/SensorEvent.html#values
                // São melhores que TYPE_MAGNETIC_FIELD e parecem mais fáceis de programar

                float[] rotation = new float[9];
                float[] orientacao_celular = new float[3];
                SensorManager.getRotationMatrixFromVector(rotation, event.values);
                SensorManager.getOrientation(rotation, orientacao_celular);
                break;
            }

            case Sensor.TYPE_MAGNETIC_FIELD: {
                float[] rotacao = new float[9], inclinacao = new float[9], orientacao_celular = new float[3];
                // float inclinacao_geomagnetico;
                boolean vazio;
                // Não enviar leituras enquanto não tivermos uma do acelerômetro
                vazio = true;
                for (float n : gravity) {
                    if (n != 0) {
                        vazio = false;
                        break;
                    }
                }
                if (vazio)
                    return;

                // getRotationMatrix retorna false se houver erro (matriz próxima de nula, por
                // exemplo, em queda livre). Também se não tivermos recebido nenhuma leitura
                // do acelerômetro ainda
                if (SensorManager.getRotationMatrix(rotacao, inclinacao, gravity, event.values)) {
                    SensorManager.getOrientation(rotacao, orientacao_celular);
                    // A seguinte linha pode servir para pegar inclinação do campo
                    // inclinacao_geomagnetico = SensorManager.getInclination(inclinacao);

                    /*log.append("Compass: yaw: " + Double.toString(orientacao_celular[0] * 180.0f / Math.PI) +
                            "\npitch: " + Double.toString(orientacao_celular[1] * 180.0f / Math.PI) +
                            "\nroll: " + Double.toString(orientacao_celular[2] * 180.0f / Math.PI) +
                            "\nincl: " + Double.toString(inclinacao_geomagnetico * 180.0f / Math.PI));*/
                }
                break;
            }

            default:
            log.append("onSensorChanged: Sensor desconhecido recebido\n");
        }
    }

    @Override
    public void onAccuracyChanged(Sensor s, int accuracy) {
        log.append(s.getName() + ": acuracia " + Integer.toString(accuracy) + "\n");
    }
}
