function test()
	print('lua:test()')
	return 'Frenk', 22, 87.5
end

function person_data(name)
	print('lua:person_data(name=' .. name .. ')')
	if name == 'Frenk Black' then
		return 25, 1890
	else
		return -1, -1
	end
end

function dummy_table()
	print('lua:dummy_table()')
	return {1, 2, 3, 2, 1}
end

function simple_error()
	f:write('write somthing somewere')
end

function filter(d)
	fd = {}
	for k,v in ipairs(d) do
		fd[k] = v*v;
	end
	return fd
end
