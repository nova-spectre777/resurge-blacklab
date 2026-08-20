package dev.resurge.plugin;

public interface FormatProbe {
    String id();
    ProbeResult inspect(byte[] region);
}
