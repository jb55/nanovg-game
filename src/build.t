
local target = arg[1]

require(target)

-- TODO(jb55) make generic
-- TODO(jb55) target arch
terralib.saveobj(target .. ".o", {})
