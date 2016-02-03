local expression = function(self,lex)
  local sum = 0
  lex:expect("sum") --first token should be "sum"
  if not lex:matches("done") then
    repeat
      --parse a number, return its value
      local v = lex:expect(lex.number).value
      sum = sum + v
    --if there is a comma, consume it and continue
    until not lex:nextif(",")
  end

  lex:expect("done")
  --return a function that is run
  --when this expression would be evaluated by Lua
  return function(environment_function)
    return sum
  end
end

local sumlanguage = {
  name = "sumlanguage"; --name for debugging
  -- list of keywords that will start our expressions
  entrypoints = {"sum"};
  keywords = {"done"}; --list of keywords specific to this language
   --called by Terra parser to enter this language
  expression = expression;
}

return sumlanguage
