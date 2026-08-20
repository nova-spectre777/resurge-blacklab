import unittest
from resurge_lab.corruption import corrupt
from resurge_lab.arena import build_fixture, recover_fixture, benchmark
class LabTests(unittest.TestCase):
    def test_corruption_is_deterministic(self):
        a,_=corrupt(b"x"*100,rate=.1,seed=3,cluster=2); b,_=corrupt(b"x"*100,rate=.1,seed=3,cluster=2); self.assertEqual(a,b)
    def test_clean_fixture_recovers_exact(self):
        image,truths=build_fixture(blocks=4,payload_size=16); out,labels=recover_fixture(image,truths,payload_size=16); self.assertEqual(out,b"".join(t.payload for t in truths)); self.assertNotIn("UNKNOWN",labels)
    def test_benchmark_never_claims_wrong_bytes(self):
        r=benchmark(rate=.03,seed=17,cluster=8); self.assertEqual(r.wrong_reconstructed_bytes,0); self.assertGreater(r.exact_recovered_bytes,0)
if __name__=="__main__": unittest.main()

class OverlayTests(unittest.TestCase):
    def test_overlay_is_non_destructive_and_bound_to_base(self):
        from resurge_lab.overlay import Patch, make_overlay, apply_overlay
        base=b"abcdefgh"; ov=make_overlay(base,[Patch(2,"5a5a","PROVEN",("crc",))]); self.assertEqual(base,b"abcdefgh"); self.assertEqual(apply_overlay(base,ov),b"abZZefgh")
        with self.assertRaises(ValueError): apply_overlay(b"xxxxxxxx",ov)
