package dev.resurge.plugin;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;

public final class BuiltinProbe implements FormatProbe {
    @Override public String id(){ return "builtin-structure-probe/v1"; }
    @Override public ProbeResult inspect(byte[] b){
        var c=new ArrayList<String>();
        if (starts(b,new byte[]{(byte)0xFF,(byte)0xD8,(byte)0xFF})) { c.add("jpeg-soi"); return new ProbeResult("jpeg",.95,c); }
        if (starts(b,"%PDF-".getBytes(StandardCharsets.US_ASCII))) { c.add("pdf-header"); return new ProbeResult("pdf",.95,c); }
        if (starts(b,new byte[]{0x50,0x4b,0x03,0x04})) { c.add("zip-local-header"); return new ProbeResult("zip",.90,c); }
        if (starts(b,"SQLite format 3\0".getBytes(StandardCharsets.US_ASCII))) { c.add("sqlite-header"); return new ProbeResult("sqlite",.99,c); }
        return new ProbeResult("unknown",0.0,c);
    }
    private static boolean starts(byte[] a, byte[] p){ if(a.length<p.length)return false; for(int i=0;i<p.length;i++)if(a[i]!=p[i])return false; return true; }
}
