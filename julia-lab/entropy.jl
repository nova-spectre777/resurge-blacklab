# Experimental Julia research notebook-equivalent kept dependency-free.
function normalized_entropy(p)
    q = filter(x -> x > 0, p)
    length(q) <= 1 && return 0.0
    s = sum(q)
    q = q ./ s
    h = -sum(x * log2(x) for x in q)
    return h / log2(length(q))
end

function acquisition_priority(info_gain, impact, uniqueness, stress)
    return max(info_gain,0) * (0.5 + max(impact,0)) * (0.5 + max(uniqueness,0)) / max(stress,0.02)
end
