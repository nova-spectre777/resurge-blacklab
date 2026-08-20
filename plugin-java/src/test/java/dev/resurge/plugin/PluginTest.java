package dev.resurge.plugin;
import java.nio.charset.StandardCharsets;
public final class PluginTest {
 public static void main(String[] args){ var p=new BuiltinProbe(); var r=p.inspect("SQLite format 3\0anything".getBytes(StandardCharsets.US_ASCII)); if(!r.format().equals("sqlite")||r.confidence()<.9)throw new AssertionError(r); var u=p.inspect(new byte[]{1,2,3}); if(!u.format().equals("unknown"))throw new AssertionError(u); System.out.println("plugin-java tests passed"); }
}
