/- Experimental Lean 4 model for the meaning of a unique recovery certificate.
   The production project must never label a region PROVEN merely because an ML/heuristic score is high. -/

def Candidate := Nat

def Valid (constraints : Candidate → Bool) (c : Candidate) : Prop := constraints c = true

def UniqueRecovery (constraints : Candidate → Bool) (chosen : Candidate) : Prop :=
  Valid constraints chosen ∧ ∀ c, Valid constraints c → c = chosen

theorem unique_recovery_excludes_other
  (constraints : Candidate → Bool) (chosen other : Candidate)
  (h : UniqueRecovery constraints chosen) (ho : Valid constraints other) :
  other = chosen := by
  exact h.2 other ho
