package configuration;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class ReadConfiguration
{
    private static Properties properties = null;

    public static void load(String configFile)
    {
        try
        {
            properties = new Properties(); //properties contiendra ttes les clés/valeurs
            File file = new File(configFile);
            if (file.exists())
            {
                FileInputStream fis = new FileInputStream(file);
                properties.load(fis);
                fis.close();
            }
        }
        catch (IOException e)
        {
            System.out.println("Erreur : " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static String get(String key)
    {

        return properties.getProperty(key);
    }

    public static int getInt(String key)
    {

        return Integer.parseInt(get(key));
    }
}