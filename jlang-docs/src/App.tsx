import { Routes, Route, Navigate } from 'react-router-dom';
import Layout from './components/Layout';
import Preface from './pages/Preface';
import Introduction from './pages/Introduction';
import TechnologiesUsed from './pages/TechnologiesUsed';
import GettingStarted from './pages/GettingStarted';
import Comments from './pages/Comments';
import Variables from './pages/Variables';
import Types from './pages/Types';
import Operators from './pages/Operators';
import ControlFlow from './pages/ControlFlow';
import Functions from './pages/Functions';
import Arrays from './pages/Arrays';
import Structs from './pages/Structs';
import Pointers from './pages/Pointers';
import NullSafety from './pages/NullSafety';
import Memory from './pages/Memory';
import Generics from './pages/Generics';
import ErrorHandling from './pages/ErrorHandling';
import Ending from './pages/Ending';

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route path="/preface" element={<Preface />} />
        <Route path="/" element={<Introduction />} />
        <Route path="/technologies-used" element={<TechnologiesUsed />} />
        <Route path="/getting-started" element={<GettingStarted />} />
        <Route path="/comments" element={<Comments />} />
        <Route path="/variables" element={<Variables />} />
        <Route path="/types" element={<Types />} />
        <Route path="/operators" element={<Operators />} />
        <Route path="/control-flow" element={<ControlFlow />} />
        <Route path="/functions" element={<Functions />} />
        <Route path="/arrays" element={<Arrays />} />
        <Route path="/structs" element={<Structs />} />
        <Route path="/pointers" element={<Pointers />} />
        <Route path="/null-safety" element={<NullSafety />} />
        <Route path="/memory" element={<Memory />} />
        <Route path="/generics" element={<Generics />} />
        <Route path="/error-handling" element={<ErrorHandling />} />
        <Route path="/ending" element={<Ending />} />
        <Route path="*" element={<Navigate to="/" replace />} />
      </Route>
    </Routes>
  );
}
