
C = require 'c'

local ctxerr = function()
  error("nvg context not bound, use: 'ctx nvg'")
end

local resolve = function(expr, env)
  if type(expr) == "function" then
    return expr(env)
  end
  return expr
end


local parseCircle = function(self, lex)
  local x = lex:luaexpr()
  local y = lex:luaexpr()
  local r = lex:luaexpr()
  return function (ctx, env)
    if not ctx then ctxerr() end
    return `C.nvgCircle([ctx]
      , [ resolve(x, env) ]
      , [ resolve(y, env) ]
      , [ resolve(r, env) ])
  end
end

local function parseRGBA(self, lex)
  local r = lex:luaexpr()
  local g = lex:luaexpr()
  local b = lex:luaexpr()
  local a = lex:luaexpr()
  return function (ctx, env)
    if not ctx then ctxerr() end
    return `C.nvgRGBA(
          [ resolve(r, env) ]
        , [ resolve(g, env) ]
        , [ resolve(b, env) ]
        , [ resolve(a, env) ]
        )
  end
end

local function parseFillColor(self, lex)
  local color = lex:luaexpr()
  return function (ctx, env)
    if not ctx then ctxerr() end
    return `C.nvgFillColor([ctx], [color(env)])
  end
end

local function parseGeneric(self, lex, expr)
  return function (ctx, env)
    return `[expr]([ctx])
  end
end

local function parseCall(self, lex)
  local expr = lex:luaexpr()
  return function (ctx, env)
    return expr(env)
  end
end

local statement = function(self, lex)
  local exprfn
  local ctxname
  if lex:nextif("ctx") then
    ctxname = lex:expect(lex.name).value
    lex:ref(ctxname)
  elseif lex:nextif("circle") then
    exprfn = parseCircle(self, lex)
  elseif lex:nextif("fillcolor") then
    exprfn = parseFillColor(self, lex)
  elseif lex:nextif("fill") then
    exprfn = parseGeneric(self, lex, `C.nvgFill)
  elseif lex:nextif("beginpath") then
    exprfn = parseGeneric(self, lex, `C.nvgBeginPath)
  elseif lex:nextif("call") then
    exprfn = parseCall(self, lex)
  end
  return function (envfn)
    local env = envfn()
    local ctx
    if not ctxname and self.ctx then
      ctx = self.ctx
    else
      ctx = env[ctxname]
      self.ctx = ctx
    end
    if exprfn then
      if not self._exprs then
        self._exprs = {}
      end
      table.insert(self._exprs, exprfn(ctx, env))
    end
  end
end

local expression = function(self, lex)
  local exprfn
  if lex:nextif("done") then
    return function (envfn)
      return self._exprs
    end
  elseif lex:nextif("rgba") then
    exprfn = parseRGBA(self, lex)
  end
  return function (envfn)
    local env = envfn()
    local ctx = self.ctx
    if exprfn then
      return exprfn(ctx, env)
    end
    return quote end
  end
end

local nvglang = {
  name = "nvglang"; --name for debugging
  -- list of keywords that will start our expressions
  entrypoints = {"done", "call", "ctx", "circle", "fill", "fillcolor", "beginpath", "rgba"}; --list of keywords specific to this language
  keywords = {"akeyword"}; --list of keywords specific to this language
   --called by Terra parser to enter this language
  expression = expression;
  statement = statement;
}

return nvglang
