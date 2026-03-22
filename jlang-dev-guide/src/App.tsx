import { Routes, Route, Navigate } from 'react-router-dom';
import Layout from './components/Layout';
import Introduction from './pages/Introduction';

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route path="/" element={<Introduction />} />
        <Route path="*" element={<Navigate to="/" replace />} />
      </Route>
    </Routes>
  );
}
