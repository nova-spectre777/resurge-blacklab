// Experimental tiny evidence adapter for constrained/portable workers.
const std = @import("std");
pub const Probe = struct { confidence: f64, recognized: bool };
pub fn probeSQLite(bytes: []const u8) Probe {
    const magic = "SQLite format 3\x00";
    return .{ .confidence = if (std.mem.startsWith(u8, bytes, magic)) 0.99 else 0.0,
              .recognized = std.mem.startsWith(u8, bytes, magic) };
}
test "sqlite magic" { try std.testing.expect(probeSQLite("SQLite format 3\x00abc").recognized); }
